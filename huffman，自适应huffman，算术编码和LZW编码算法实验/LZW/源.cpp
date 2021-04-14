#include <string>
#include <map>
#include <math.h>
#include <iostream>
#include <iterator>
#include<stdio.h>
#include <vector>
#include<sys/timeb.h>
#include <time.h>
using namespace std;
void output_dic(const string &uncompressed,map<string,int> Dic) {

	//output code of SINGLE character
	int i = 1, j = 0;
	int n = 0;
	char s[80], word[80];
	//output them  avoiding the repeated items
	for (string::const_iterator it = uncompressed.begin(); it != uncompressed.end(); ++it, ++n)
	{
		word[n] = *it;//n+1 characters
	}
	s[0] = word[0];
	int k = 1;
	for (i = 0; i < n + 1; i++)
	{
		for (int j = 0; j < k; j++)
		{
			if (word[i] == s[j]) break;
			else if ((word[i] != s[j]) && (j == k - 1))
			{
				s[k] = word[i];
				k++;//k= number of characters of b
			}
		}//ONLY k distinguished characters
	}
	//output akk single codes without repeat
	s[k] = '\0';
	for (j = 0; j < k - 1; j++)
		cout << "\t " << s[j] << ":" << Dic[string(1, s[j])] << "\n";

}

void compress(const string &uncompressed, vector<int>& vec)
{
	//get the start time of compression
	struct timeb time1, time2;
	ftime(&time1);
	long start = 1000 * time1.time + time1.millitm;

	// intialize the dictionary.
	int dictSize = 256;
	map<string, int> dictionary;
	for (int i = 0; i < 256; i++)
	{
		dictionary[string(1, i)] = i;
	}
	//output the code of single character in dictionary
	output_dic(uncompressed, dictionary);

	string w;
	for (string::const_iterator it = uncompressed.begin(); it != uncompressed.end(); ++it)
	{
		char c = *it;
		string wc = w + c;
		//if the symbol is in the dictionary
		if (dictionary.count(wc))
		{
			w = wc;
			dictionary[w];
		}
		else {
			vec.push_back(dictionary[w]);
			// Add wc to the dictionary.
			dictionary[wc] = dictSize++;

			cout << "\t" << wc << ":" << dictionary[wc] << endl;//output the code of added items

			w = string(1, c);
		}
	}

	//get the end time of compression
	ftime(&time2);
	long end = 1000 * time2.time + time2.millitm;


	if (!w.empty())
		vec.push_back(dictionary[w]);
	
	//output the time cost of compression
	cout << "Compression time cost:" << end - start << "ms\n";
	

}

int main() {
	vector<int> compressed;
	vector<int>::iterator it;
	string::const_iterator str;
	cout << "----------------------LZW coding-----------------------------\n";
	cout << "Please input an string \n\t";
	string s;
	cin >> s;
	cout << "--------------------------------------------" << endl;
	cout << "The dictionary list is:\n";
	compress(s, compressed);
	cout << "--------------------------------------------" << endl;

	cout << "The whole compressed code is:\n";
	copy(compressed.begin(), compressed.end(), ostream_iterator<int>(cout, ","));


	//output Compressed Ratio
	int B1 = 0, B2 = 0, singleBit;
	//the bits before compression
	B2 = 8 * s.length();
	//the bits after compression
	for (it = compressed.begin(); it != compressed.end(); it++) {

		//calculate the bits of added items in dictionary by log2(x)
		if (*it >= 256) {
			singleBit = (int)log2(*it) + 1;
			B1 += singleBit;
		}
		else B1 += 8;//the ASCII are all 8bits
	}
	cout << "\nCompression Ratio:" << (double)(B2-B1) / B1 << "\n\n";
	return 0;
}