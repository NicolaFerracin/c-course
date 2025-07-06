#include <stdio.h>

void incr_local(void) {
    int x = 0;
    x = x + 1;
    printf("X: %d\n", x);
}

int y = 0;
void incr_global(void) {
    y = y + 1;
    printf("Y: %d\n", y);
}

void incr_static(void) {
    static int z = 0;
    z = z + 1;
    printf("Z: %d\n", z);
}

int main() {
    printf("incr_local prints always 1\n");
    incr_local();
    incr_local();
    incr_local();
    
    printf("incr_global prints 1 2 3\n");
    incr_global();
    incr_global();
    incr_global();
    
    printf("incr_static uses a local static variable, and prints 1 2 3\n");
    incr_static();
    incr_static();
    incr_static();

    return 0;
}