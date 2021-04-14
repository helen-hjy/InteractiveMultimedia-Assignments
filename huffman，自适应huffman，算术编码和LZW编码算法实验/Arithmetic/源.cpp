#include<iostream>
#include<string>
#include<cstring>
#include<time.h>
#include<sys/timeb.h>
#include<vector>
using namespace std;
#define N 50
int B = 0;
struct L {
	char ch;
	int num;
	double f;
};
string decimal(double m);
int proba(string str, char c[], long double p[], int count);
int search(vector<L> arch, char ch1, int n);
long double encode(char c[], long double p[], string str, int number, int size);

//因为编码是小数所以需要转换乘2进制
string decimal(double m)
{
	string s="0.";
	int inte;
	while (m != 1) {
		m = m * 2;
		inte = (int)m;
		if(inte==1|inte==0)
		   s += (char)inte;
		m = (double)m;
		if (s.length() == 1000)  break;
	}
	cout <<"length:"<< s.length()<<"\n";
	return s;

}

//得到每一个字符的概率
int proba(string str, char c[], long double p[], int count) {
	//c[]是字符数组，p[]是该字符概率数组
	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(3);

	vector<L> pt;
	L temp;
	temp.ch = str[0];
	temp.num = 1;
	temp.f = 0.0;
	pt.push_back(temp);
	for (int i = 1; i < count; i++) {
		temp.ch = str[i];
		temp.num = 1;
		temp.f = 0.0;

		for (int j = 0; j < pt.size(); j++)
		{
			int k = search(pt, str[i], pt.size());
			if (k >= 0)
			{
				pt[k].num++;
				break;
			}
			else {
				pt.push_back(temp);
				break;
			}
		}

	}
	for (int i = 0; i < pt.size(); i++)
		pt[i].f = double(pt[i].num) / count;
	int number = pt.size();
	cout << "Probability of single character:\n";

	for (int i = 0; i < number; i++) {
		if (count == 0) {
			cout << "NO smaple!\n";
		}
		else
		{
			c[i] = pt[i].ch;
			p[i] = pt[i].f;
			cout <<"\t"<< c[i] << ":" << p[i] << endl;
		}
	}
	return number;
}

int search(vector<L>arch, char ch1, int n)
{
	for (int i = 0; i < n; i++)
		if (ch1 == arch[i].ch)
			return i;

	return -1;
}
long double encode(char c[], long double p[], string str, int number, int size)
{
	long double High = 0.0, Low = 0.0, high, low, range;
	int i, j = 0;
	//给第一个读入的字符安排区间
	for (i = 0; i < number; i++)
		if (str[0] == c[i]) break;
	while (j < i)
		Low += p[j++];
	range = p[j];
	High = Low + range;

	//开始判断每一个字符并更新概率区间
	for (i = 1; i < size; i++)
		for (j = 0; j < number; j++)
		{
			if (str[i] == c[j])
			{
				if (j == 0)
				{
					low = Low;
					high = Low + p[j] * range;
					High = high;
					range *= p[j];
				}
				else {
					float proba_next = 0.0;
					for (int k = 0; k <= j - 1; k++)
						proba_next += p[k];

					low = Low + range * proba_next;
					high = Low + range * (proba_next + p[j]);
					Low = low;
					High = high;
					range *= p[j];

				}
			}
			else continue;
		}
	//输出总编码
	cout << endl;
	cout << "  The whole code is:" << Low << endl;
	return Low;
}

int main() {
	string str;
	string::const_iterator strItr;
	int number = 0, size = 0;
	int B1=0, B2 = 0;
	char c[N];
	long double p[N], output;

	cout << "---------------Arithmetic coding-----------------" << endl;
	cout << "Please input a string:";
	cin >> str;
	size = str.length();
	number = proba(str, c, p, size);

	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(10);

	//get start time
	struct timeb time1, time2;
	ftime(&time1);
	long start = 1000 * time1.time + time1.millitm;

	output = encode(c, p, str, number, size);

	//get the end time of compression
	ftime(&time2);
	long end = 1000 * time2.time + time2.millitm;
	//get the compression time
	long dura = end - start;


	cout << "--------------------------------------------\n";
	cout << "           Coding Finished!" << endl;
	//the bits after compression
	cout << "Binary code:";
	string s = decimal(output);
	cout << s;
	
	cout << "\nCompression time cost:" << dura << "ms\n";
	//the bits before compression
	for (strItr = str.begin(); strItr != str.end(); strItr++)
		B2 += 8;
	
	cout << "Compression Ratio:" << (double)(B2-B1) / B2 << endl;


	return 0;
}
