#include <stdio.h>

void dereferencing() {
    int x = 5;

    printf("x is: %d\n", x);

    // change value of x using the pointer and accessing [0]
    int *y = &x;
    // y[0] is equivalent to *y, since y points to a single int
    y[0] = y[0] + 1; 
    printf("x is now: %d\n", x);
    
    // change value of x using the pointer and *
    *y = *y + 1;
    printf("and x is finally: %d\n", x);
}

void stackedPointers() {
    int x = 10;

    // pointer to x
    int *y = &x;
    printf("x is stored at %p\n", y);

    // pointer to y
    int **z = &y;
    printf("y is stored at %p\n", z);
}

void pointerSize() {
    int x = 10;
    printf("x size: %d bytes - x pointer size: %d bytes\n", (int)sizeof(x), (int)sizeof(&x));
}

void strings() {
    char hello[] = "Hello world!";
    
    // hello is a pointer itself, no need to add & in front
    printf("String stored at %p with value %s\n", hello, hello);

    // because of the above we can also store it in a variable like this. NOTE we are not doing &hello
    char *p = hello;

    // using dereferencing to print any character from the string
    printf("%c %c %c\n", p[0], p[1], p[2]);         // array-style
    printf("%c %c %c\n", *p, *(p+1), *(p+2));       // pointer arithmetic
}

void shortPointer() {
    char hello[] = "Hello world!";
    char *p = hello;
    printf("1 byte pointer, %c\n", *p); // H - because remember that %c prints the character corresponding to the ASCII value of the byte

    short *s = (short*)hello; // using casting to short* to avoid warnings
    printf("2 bytes pointer, %d\n", *s); // 25928 - contrary to %c, with the short pointer, we get the actual integer stored in the 2 bytes that we are pointing to
    // 'H' is 72 and 'e' is 101 => 72 * 1 + 101 * 256 (1 byte = 256 bit) => 25928

    // another cool thing: incrementing the pointer, multiplies the increment by the base size
    p++; // will point to 'e'
    s++; // will not point to 'el' but will point to 'll'
    printf("1 byte pointer, %c\n", *p); // e
    printf("2 bytes pointer, %d\n", *s); // 27756 => l = 108 => 108 + 108*256 => 27756!
}

int main() {
    dereferencing();
    printf("=======================\n");
    stackedPointers();
    printf("=======================\n");
    pointerSize();
    printf("=======================\n");
    strings();
    printf("=======================\n");
    shortPointer();

    return 0;
}