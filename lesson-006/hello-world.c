#include <stdio.h>

void count(int start, int end) {
    if (start > end) return;
    printf("%d\n", start);
    printf("start: %p; end: %p\n", &start, &end); // you can see start and end take more and more memory
    count(start + 1, end);
}

int main() {
    // BLOCKS
    int i = 8;
    {
        int i = 5;
        printf("%d\n", i); // i is 5
    }
    printf("%d\n", i); // i is 8

    // GOTO
    int x = 0;
again:
    printf("%d\n", x);
    x++;
    if (x < 10) goto again;

    // RECURSION
    count(0, 9);

    // BASIC-LIKE
// l10: printf("infinite loop\n");
// l20: goto l10;
//     return 0;
}