#include <stdio.h>
#include <stdlib.h>

// MALLOC NAIVE
void set_fraction(int* f, int num, int den) {
    f[0] = num;
    f[1] = den;
}
void print_fraction(int* f) {
    printf("%d/%d\n", f[0], f[1]);
}
int* create_fraction(int num, int den) {
    int* f = malloc(sizeof(int) * 2);
    if (f == NULL) return NULL; // Malloc error checking. If there is no memory, we get NULL
    f[0] = num;
    f[1] = den;
    return f;
}
void simplify_fraction(int* f) {
    for (int i = 2; i <= f[0] && i <= f[1]; i++) {
        while (f[0] % i == 0 && f[1] % i == 0) {
            f[0] /= i;
            f[1] /= i;
        }
    }
}
void fraction_malloc() {
    int* f = malloc(sizeof(int) * 2); // allocate space for two integers
    set_fraction(f, 1, 2);
    print_fraction(f); // prints 1/2

    // now if we want a new fraction variable, we need to allocate malloc again and it becomes quickly annoying and unmanageable
    // to avoid this, we can use a create_fraction that takes care of the memory allocation
    int* f1 = create_fraction(4, 5);
    int* f2 = create_fraction(10, 20);
    int* f3 = create_fraction(4, 4);
    print_fraction(f1); // prints 4/5
    print_fraction(f2); // prints 10/20
    print_fraction(f3); // prints 4/4

    // we can now move onto creating a simplify_fraction helper to simplify the fraction terms
    simplify_fraction(f1);
    simplify_fraction(f2);
    simplify_fraction(f3);
    print_fraction(f1); // prints 4/5
    print_fraction(f2); // prints 1/2
    print_fraction(f3); // prints 1/1

    // The above is all good and dandy but it's not really a nice devx having to constantly access f[0] and f[1] ====> let's use structs!
}

// EXPERIMENTING WITH STRUCTS
struct my_struct {
    int x;
    int y;
};
struct my_struct_2 {
    int x;
    int y;
    unsigned char z;
};
struct my_struct_3 {
    int x;
    int y;
    unsigned char z;
    unsigned char zz;
    unsigned char zzz;
    unsigned char zzzz;
};
struct my_struct_4 {
    unsigned char z;
    int x;
    unsigned char zz;
    int y;
    unsigned char zzz;
    unsigned char zzzz;
};
void structs_playground() {
    printf("struct my_struct { int x; int y; }\n");
    printf("my_struct size: %d bytes\n", (int)sizeof(struct my_struct));
    printf("In fact it holds 2 ints, where an int takes %d bytes\n\n", (int)sizeof(int));

    printf("struct my_struct_2 { int x; int y; unsigned char z; }\n");
    printf("my_struct_2 size: %d bytes\n", (int)sizeof(struct my_struct_2));
    printf("The additional unsigned char takes 4 bytes instead of 1!\n");
    printf("This happens due to struct padding: each member is aligned to memory addresses that are multiples of the largest member's size for efficient access\n");
    printf("In our case above, the unsigned char takes 4 bytes, one of which is the actual value and the rest is all padding\n\n");

    printf("But C is smart enough so that if you have multiple smaller members, it will group them together.\n");
    printf("Example (2 ints + 4 chars):\n");
    printf("struct my_struct_3 { int x; int y; unsigned char z; unsigned char zz; unsigned char zzz; unsigned char zzzz; }\n");
    printf("my_struct_3 size: %d bytes\n", (int)sizeof(struct my_struct_3));
    printf("my_struct_3 is 12 bytes like my_struct_2, despite having 3 more unsigned chars\n\n");

    printf("The grouping of smaller members does not work if they are dispersed in the struct.\n");
    printf("struct my_struct_4 { unsigned char z; int x; unsigned char zz; int y; unsigned char zzz; unsigned char zzzz; }\n");
    printf("my_struct_4 size: %d bytes\n", (int)sizeof(struct my_struct_4));
    printf("my_struct_4 is 20 as the 4 unsigned chars could not be grouped together and end up taking 4 bytes each.\n\n");
}

// FRACTION STRUCT
struct fraction {
    int num;
    int den;
};
void print_fraction_2(struct fraction* f) {
    printf("%d/%d\n", f->num, f->den);
}
struct fraction* create_fraction_2(int num, int den) {
    struct fraction* f = malloc(sizeof(*f));
    if (f == NULL) return NULL; // Malloc error checking. If there is no memory, we get NULL
    f->num = num;
    f->den = den;
    return f;
}
void simplify_fraction_2(struct fraction* f) {
    for (int i = 2; i <= f->num && i <= f->den; i++) {
        while (f->num % i == 0 && f->den % i == 0) {
            f->num /= i;
            f->den /= i;
        }
    }
}
struct fraction create_fraction_return_value(int num, int den) {
    struct fraction f;
    f.num = num;
    f.den = den;
    return f;
}
void fraction_struct() {
    printf("When using the struct variable directly, we use '.' to access members.\n");
    struct fraction f;
    f.num = 10;
    f.den = 20;
    printf("f.num: %d - f.den: %d\n\n", f.num, f.den);

    printf("When using a pointer to a struct, we use '->' to access members.\n");
    struct fraction* ff = &f;
    printf("ff->num: %d - ff->den: %d\n\n", ff->num, ff->den);

    printf("We can now reimplement the fraction setup we did with malloc, but using structs instead.\n");
    struct fraction* f1 = create_fraction_2(10, 20);
    print_fraction_2(f1); // prints 10/20
    simplify_fraction_2(f1);
    print_fraction_2(f1); // prints 1/2
    printf("\n\n");

    printf("Interestingly, structs are handled as a different data type than the usual int.\n");
    printf("This means we could make `create_fraction` return the actual fraction variable value, not its pointer. See `create_fraction_return_value` for details.\n");
    struct fraction f2 = create_fraction_return_value(10, 20);
    print_fraction_2(&f2); // prints 10/20
    simplify_fraction_2(&f2);
    print_fraction_2(&f2); // prints 1/2
    printf("What C does, is that it copies the struct returned by `create_function_return` into a new struct of the same type. Having a program with several big structs returned that get copied is highly inefficient, that's why structs are usually returned as pointers.\n");


}

int main() {
    printf("To explore structs, let's use a `fraction` data structure\n");
    printf("Let's first implement it naively using malloc\n");
    fraction_malloc();
    printf("\n################################\n\n");

    printf("Let's experiment with structs!\n");
    structs_playground();
    printf("\n################################\n\n");

    printf("Let's use structs instead!\n");
    fraction_struct();
}