#include<iostream>
#include<fstream>

using namespace std;

int main(int argc, char** argv)
{
    ofstream fout("output.txt");
    int n = 0;
    if (argc >= 2) n = stoi(argv[1]);
    else 
    {
        cout << "n is uknown\n";
        return 0;
    }
    if (argc >= 2) for(int i = 1; i <= n; ++i) fout << i << " ";
    fout.close();
    return 0;
}