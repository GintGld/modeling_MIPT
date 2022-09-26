#include<iostream>
#include<fstream>
#include<array>

using namespace std;

int main(int argc, char** argv)
{
    ofstream fout("output.txt");
    int const max_size = 100;
    array <int, max_size> a;
    
    int n = 0;
    if (argc >= 2) n = stoi(argv[1]);
    else
    {
        cout << "n is not declared";
        return 0;
    }
    a[0] = 1;
    a[1] = 1;
    for (int i = 2; i < n; ++i) a[i] = a[i - 1] + a[i - 2];
    for (int i = 0; i < n; ++i) fout << i + 1 << ' ' << a[i] << "\n";
    fout.close();
    return 0;
}