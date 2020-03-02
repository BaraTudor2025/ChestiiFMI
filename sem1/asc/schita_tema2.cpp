#include <iostream>
using namespace std;

int suma_patrate(int num) {
    num -= 1;
    if(num == 1)
        return 1;
    else{
        int suma = suma_patrate(num);
        return num * num + suma;
    }
}

void modifica(int v[], int n)
{
    v[n] = suma_patrate(v[n]);
    if(n > 0)
        modifica(v, n-1);
}

void modif(int* v, int n){
    *v = suma_patrate(*v);
    if(n != 0)
        modif(v, n-1);
}

int main()
{
    int n=5;
    int v[100] = {3,9,5,6,7};
    modifica(v, n-1);
    for(int i=0;i<n;++i)
        cout<<v[i]<<" ";

        //return suma_patrate(4);
}