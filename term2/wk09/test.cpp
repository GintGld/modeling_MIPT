#include<iostream>
int foo(){
    return(sizeof(double));
}

int main() {
    int Nx = foo();
    double **a = new *double[Nx];
    double a[Nx][Nx];
    a[9][0] = 1.0;
    std::cout << "fine" << **a << std::endl;

}