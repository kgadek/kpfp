#include <iostream>
#include <cstdlib>
#include "NewHandlerSupport.hpp"

using namespace std;

void myHandler() {
    cerr << "noidea";
    abort();
}

class Bla : public NewHandlerSupport<Bla> {
    double a,b;
public:
};

int main() {
    Bla::set_new_handler(myHandler);
    cout << "a\n";
    for(int i=0;i<1000000000;++i) {
        new Bla;
    }
    std::cout << "c\n";
    return 0;
}

