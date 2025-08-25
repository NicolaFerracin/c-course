#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* create_lps(char* s, int len) {
    char* str = malloc(4 + len + 1); // 4-byte max size instead of 1
    uint32_t* lps = (uint32_t*)str;
    *lps = len;

    str += 4;
    for (int i = 0; i < len; i++) str[i] = s[i];

    str[len] = 0;

    return str;
}

void print_lps(char* s) {
    uint32_t* len = (uint32_t*)(s - 4); // as s points to the string and not the header, we need to offset
    for (int i = 0; i < (int)*len; i++) putchar(s[i]);
    printf("\n");
}

// NOT NEEDED ANYMORE
// as we return the pointer to the embedded string instead of the header, our pointer is already interoberable and we don't need lps_getc anymore
// char* lps_getc(char* s) {
//     return s + 1;
// }

// We need to free the memory including the header, so we need to account for the fact that the pointer points to the embedded string. Doing -4 to point to the header
void free_lps(char* s) {
    free(s - 4);
}

// Usually str.len is O(n). With our LPS we can do the same in O(1)
uint32_t len_lps(char* s) {
    uint32_t* len = (uint32_t*)(s - 4);
    return *len;
}

int main() {
    char* str = create_lps("Hello World", 11);
    print_lps(str); // use helper to print a LPS
    // printf("%s\n", lps_getc(str)); // we don't need the helper to get the interoperable C string anymore
    printf("%s\n", str); // we can now print our embedded string directly, without additional helpers


    free_lps(str);
    return 0;
}