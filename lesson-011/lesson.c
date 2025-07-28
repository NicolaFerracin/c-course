#include <stdio.h>

void print_zero_terminated_string() {
    char str[] = "Hello";
    char* p = str;
    while (*p) putchar(*p++);
    printf("\n");
}

void print_until_zero() {
    char str[] = "Hel\0lo";
    char* p = str;
    while (*p) putchar(*p++);
    printf("\n");
}

void print_length_prefixed_string() {
    char str[] = "\6Hel\0lo"; // we prefix the string with its own length (excluding the prefix itself BUT including the 0 byte in the middle)
    char* p = str;
    int len = *p++;
    printf("String length: %d\n", len);
    for (int i = 0; i < len; i++) putchar(p[i]);
    printf("\n");
}

int main() {
    // zero-terminated string
    print_zero_terminated_string();

    // puts a zero byte in the middle of the string
    print_until_zero();

    // length-prefixed string
    print_length_prefixed_string();

    return 0;
}