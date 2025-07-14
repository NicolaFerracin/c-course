#include <stdio.h>

int main() {
    int x = 5;
    int *y = &x;

    printf("x is at address: %p\n", y);
    printf("%d\n", x);
    
    *y = 10;
    printf("%d\n", x);

    return 0;
}