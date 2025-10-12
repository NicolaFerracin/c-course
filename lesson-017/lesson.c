#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct lps {
    uint32_t len;
    uint32_t refcount;
    uint32_t magic; // magic number to determine string validity
    char str[]; // we don't know how big this is going to be, will be determined by the call to malloc
};

char* create_lps(char* s, int len) {
    struct lps* lps = malloc(sizeof(*lps) + len + 1); // set the str length inside the lps struct to be len + 1 for the null terminator
    lps->len = len;
    lps->refcount = 1;

    for (int i = 0; i < len; i++) lps->str[i] = s[i];

    lps->str[len] = 0; // add null terminator
    lps->magic = 0xDEADBEEF; // add magic number

    return lps->str;
}

void print_lps(char* s) {
    // we get the pointer to the string, we need to get to the memory address where the whole struct is stored
    // we get to it by subtracting from the current string pointer, the size of the struct, which will bring us to the very beginning
    struct lps* lps = (struct lps*)(s - sizeof(*lps));
    for (int i = 0; i < (int)lps->len; i++) putchar(s[i]);
    printf("\n");
}

void free_lps(char* s) {
    free(s - sizeof(struct lps));
}

void validate_lps(struct lps* lps) {
    if (lps->magic != 0xDEADBEEF) {
        printf("INVALID STRING: Aborting\n");
        exit(1);
    }
}

void release_lps(char* s) {
    struct lps* lps = (struct lps*)(s - sizeof(*lps));
    validate_lps(lps);
    if (lps->refcount == 0) {
        // releasing once we already have 0 references, is an error
        printf("ABORTED ON FREE ERROR\n");
        exit(1);
    }
    lps->refcount--;
    // no more references? Let's nuke this object then!
    if (lps->refcount == 0) {
        lps->magic = 0;
        free_lps(s);
    }
}

void retain_lps(char* s) {
    struct lps* lps = (struct lps*)(s - sizeof(*lps));
    validate_lps(lps);
    if (lps->refcount == 0) {
        // retaining once we already have 0 references, is an error
        printf("ABORTED ON RETAIN OF ILLEGAL STRING\n");
        exit(1);
    }
    lps->refcount++;
}

uint32_t len_lps(char* s) {
    struct lps* lps = (struct lps*)(s - sizeof(*lps));
    return lps->len;
}

char* global_string;

int main() {
    printf("create a new string, with a refcount of 1\n");
    char* str = create_lps("Hello World", 11);
    struct lps* lps = (struct lps*)(str - sizeof(*lps));
    printf("refcount: %d\n", lps->refcount);
    printf("=================\n");

    printf("we assign a new reference, we need to call retain\n");
    global_string = str;
    retain_lps(str);
    printf("refcount: %d\n", lps->refcount);
    printf("=================\n");

    printf("Printing the 2 instances:\n");
    print_lps(str);
    print_lps(global_string);
    printf("=================\n");

    printf("now we release the first instance, refcount should be 1 again\n");
    release_lps(str);
    printf("refcount: %d\n", lps->refcount);
    printf("=================\n");

    printf("now we release all active references. Refcount should be something that does not make sense, as it was freed\n");
    release_lps(str);
    printf("refcount: %d\n", lps->refcount);
    printf("=================\n");

    printf("now, notice how we could still call retain/release, because the memory addresses where our str was, have been reallocated for something else.\n");
    printf("we can introduce the concept of a magic number we can use to check the validity of the lps string\n");
    release_lps(str);

    return 0;
}


