#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

// Read variable number of function parameters
void foo(char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    char* p = fmt;
    while (*p) {
        if (*p == 'i') {
            int i = va_arg(ap, int);
            printf("%d\n", i);
        }
        else if (*p == 's') {
            char* s = va_arg(ap, char*);
            printf("%s\n", s);
        }
        else {
            printf("Wrong specifier\n");
            goto cleanup;
        }
        p++;
    }

cleanup:
    va_end(ap);
}

// Simplify reading variable number of function parameters by using a variad wrapper around printf
void bar(char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
}

// Example of variadic printf, printing in a buffer instead of printing to the video
void baz(char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    // we allocate a buffer for the vsnprintg
    char my_buf[64];
    vsnprintf(my_buf, sizeof(my_buf), fmt, ap);
    // the above just collects things in a buffer but does not print anything yet

    // now that we have the whole buffer, let's center it on a terminal with a 80 char width
    int padding = (80 - strlen(my_buf)) / 2;
    for (int p = 0; p < padding; p++) printf(" ");
    printf("%s\n", my_buf);

    va_end(ap);
}

int main() {
    printf("Read variable number of function parameters\n\n");
    foo("iisi", 10, 20, "ciao ciao", 5);
    printf("===========================================\n\n");

    printf("Simplify reading variable number of function parameters by using a variad wrapper around printf\n\n");
    bar("%d %d %s %d\n", 10, 20, "ciao ciao", 5);
    printf("===========================================\n\n");

    printf("Collect items to print in a buffer with vsnprintf so that we can perform some actions on the printed stuff\n\n");
    baz("%d %d %s %d\n", 10, 20, "ciao ciao", 5);
    baz("%s\n", "This should be centered, right?");
    baz("%s\n", "YES");
    printf("===========================================\n\n");

    return 0;
}