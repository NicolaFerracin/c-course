#include <stdio.h>

void count(int start, int end) {
iterate: // add label
    if (start > end) return;
    printf("%d\n", start);
    // count(start + 1, end); gets removed
    start += 1; // replace start + 1
    printf("start: %p; end: %p\n", &start, &end);
    goto iterate; // start from the top
}

int main() {
    // RECURSION TO ITERATION
    count(0, 9);

    // FOR LOOP TO WHILE
    for (int i = 0; i < 10; i++) {
        printf("%d\n", i);
    }

    int i = 0;
    for (; i < 10; i++) {
        printf("%d\n", i);
    }

    int i = 0;
    for (;; i++) {
        if (i >= 10) break;
        printf("%d\n", i);
    }

    int i = 0;
    for (;;) {
        if (i >= 10) break;
        printf("%d\n", i);
        i++;
    }

    int i = 0;
    while (1) {
        if (i >= 10) break;
        printf("%d\n", i);
        i++;
    }
}