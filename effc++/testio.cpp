#include <iostream>
#include <cstdio>

using namespace std;

int main() {
    ios::sync_with_stdio(0);
    for(int i=0; i<10000000; ++i)
        //printf(".%d",i);
        cout << "." << i;
}

