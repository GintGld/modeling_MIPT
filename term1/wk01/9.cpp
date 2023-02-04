#include<iostream>
#include<fstream>
#include<array>

using namespace std;

int main(int argc, char** argv)
{
    ofstream fout("output.txt");
    int const max_size = 100;
    array <double, max_size> a;
    int n = 0;
    if (argc >= 2) n = stoi(argv[1]);
    else 
    {
        cout << "n is uknown\n";
        return 0;
    }
    for(int i = 0; i < n; ++i) a[i] = 1. / (1. + i);
    fout << scientific;
    for (int i = 0; i < n; ++i) fout << a[i] << "\n";
    fout.close();
    return 0;
}