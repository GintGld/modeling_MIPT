#include<iostream>
#include<fstream>

using namespace std;

int main()
{
    ofstream fout("output.txt");
    for(int i = 1; i <= 30; ++i) fout << i << " ";
    fout.close();
    return 0;
}