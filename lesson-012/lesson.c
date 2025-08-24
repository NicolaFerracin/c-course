#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// we confirm that C handles strings terminating with a null byte by default. 
// Adding a null byte in the middle of the string effectively ignores everything coming after it.
void check_string_with_zero_byte() {
    char str[] = "Hello\0 World";
    printf("'Hello World' len is 11 chars.\n");
    printf("But 'strlen' stops at the zero-byte right after 'Hello'. Len: %lu\n", strlen(str));
}

// This function creates a local variable and returns a pointer to it.
// The function fails to compile as 'buf' does not exist anymore after the function finishes executing, hence there is no pointer.
// char* fn_returning_pointer() {
//     char buf = 'a';
//     return &buf;
// }

// This function receives a pointer to a buffer and a string to initialise it with. 
// It stores the length of the provided string in the first byte, and the actual string right after it.
void init_lps(char* s, char* init, int len) {
    // first we cast 'char *s' to 'unsigned char* lps' as we need to store the length in the first byte, and the length is not a char but an unsigned char
    unsigned char* lps = (unsigned char*)s;
    *lps = len;
    for (int i = 0; i < len; i++) s[i + 1] = init[i];
}

// Prints a length-prefixed string to the screen.
void print_lps(char* s) {
    unsigned char* len = (unsigned char*)s;
    for (int i = 1; i <= *len; i++) putchar(s[i]);
    printf("\n");
}

// Instead of initialising a buffer like in init_lps, we create a brand new variable with `malloc`.
// The rest of the logic stays the same
char* create_lps(char* s, int len) {
    char* str = malloc(1 + len + 1);
    unsigned char* lps = (unsigned char*)str;
    *lps = len;
    for (int i = 0; i < len; i++) str[i + 1] = s[i];

    str[len + 1] = 0;

    return str;
}

char* lps_getc(char* s) {
    return s + 1;
}

int main() {
    // check string length with 0 byte in it
    check_string_with_zero_byte();

    // we can't get a pointer to a variable created within another function
    // UNCOMMENT THE FOLLOWING LINE TO SEE THE PROGRAM ERROR OUT
    // char a = fn_returning_pointer();

    // We write a naive implementation for creating a length-prefixed string (lps)
    char buf[256];
    init_lps(buf, "Hello\0World", 11);
    print_lps(buf);

    // the above is all good and dandy, but in a real program, it's not possible to handle all the buffers and strings.
    // That's why we are going to create a create_lps function that instead of taking the pointer to a buffer and fill it in,
    // it uses `malloc` to create a pointer that can then be returned
    char* str = create_lps("Hello\0World", 11);
    print_lps(str);

    // We can also make our LPS strings interoperable with the standard C strings by adding a method that returns the point to the 0-terminated string embedded in our LPS
    printf("%s\n", lps_getc(str));

    // When using malloc to allocate memory, we need to free said memory
    free(str);
    return 0;
}