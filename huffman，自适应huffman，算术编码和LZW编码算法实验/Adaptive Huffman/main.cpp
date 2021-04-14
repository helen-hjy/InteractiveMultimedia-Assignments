#include<iostream>
#include <fstream>
#include<sys/timeb.h>
#include"huffman.h"
using namespace std;

int main(){
   HuffmanTree huf;
   string binarycode;
   string input;
   cout << "-------------------Adaptive huffman coding---------------" << endl;
   cout << "Please input a string:";
   cin >> input;
   //get start time
   struct timeb time1, time2;
   ftime(&time1);
   long start = 1000 * time1.time + time1.millitm;
  //encode
   binarycode=huf.encode(input);

  //get the end time of compression
   ftime(&time2);
   long end = 1000 * time2.time + time2.millitm;
   //get the compression time
   long dura = end - start;
   cout << "----------------------------------------------------\n";
   cout << "The whole code:" << binarycode<<endl;
   cout << "Compression time cost:" << dura << "ms\n";

   //get the compression ratio
   int B1 = 0, B2 = 0;
   //B2=bits before encode
   B2 = input.length() * 8;
   //B1=bits after encode
   B1 = binarycode.length();
   cout <<"Comparison ratio is: "<<(double)(B2-B1)/ B2;

   return 0;

}