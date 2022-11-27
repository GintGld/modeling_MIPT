#include<iostream>
#include<fstream>
#include<vector>

using namespace std;

int main(int argc, char** argv)
{
    ofstream fout("output.bin", ios::binary);
    vector <double> v;
    int n = 0;
    if (argc >= 2) n = stoi(argv[1]);
    else 
    {
        cout << "n is uknown\n";
        return 0;
    }
    for(int i = 0; i < n; ++i) v.push_back(1. / (1. + i));
    fout << scientific;
    for (double k : v) fout << k << "\n";
    fout.close();
    return 0;
}