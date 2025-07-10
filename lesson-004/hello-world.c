#include <stdio.h>

void clear(void) {
    printf("\x1b\x5b\x33\x4a\x1b\x5b\x48\x1b\x5b\x32\x4a");
}

int main() {
    clear();
    printf("hello wossrld\n");
    return 0;
}
