#include <cstdio>

template <int V>
struct SUM {
    enum { val = V + SUM<V-1>::val };
};

template <>
struct SUM<0> {
    enum { val = 0 };
};

int main(int argc, char **argv) {
    const int s = 1000;
    printf("sum 0..%d = %d\n", s, SUM<s>::val);
    return 0;
}

