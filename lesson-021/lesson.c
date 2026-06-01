#include <stdio.h>

struct foo {
    int i;
    unsigned char a[4];
};

struct ufoo {
    union {
        int i;
        unsigned char a[4];
    };
};

void unions() {
    struct foo f = { 10, { "abcd" } };
    printf("%d [%d %d %d %d]\n", f.i, f.a[0], f.a[1], f.a[2], f.a[3]);

    struct ufoo uf = { 10 };
    printf("%d [%d %d %d %d]\n", uf.i, uf.a[0], uf.a[1], uf.a[2], uf.a[3]);
};

struct bar {
    unsigned char a : 4;
    unsigned char b : 4;
    unsigned char c : 8;
};

void bitfields() {
    struct bar b;
    b.a = 17;
    b.b = 15;
    b.c = 15;
    printf("Size of b: %zu\n", sizeof(b));
    printf("%d %d %d\n", b.a, b.b, b.c);
}


int main() {
    unions();
    bitfields();
    return 0;
}