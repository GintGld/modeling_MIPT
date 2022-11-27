#include<iostream>
#include<fstream>

using namespace std;

int main()
{
    ofstream fout("output.txt");
    int n;
    cin >> n;
    for(int i = 1; i <= n; ++i) fout << i << " ";
    fout.close();
    return 0;
}