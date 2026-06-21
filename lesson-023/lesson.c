#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* =========== DATA STRUCTURES =========== */

#define TFOBJ_TYPE_INT 0
#define TFOBJ_TYPE_STR 1
#define TFOBJ_TYPE_BOOL 2
#define TFOBJ_TYPE_LIST 3
#define TFOBJ_TYPE_SYMBOL 4

typedef struct tfobj {
    int refcount; // Reference counting
    int type; // TFOBJ_TYPE_*
    union {
        int i; // We can use 1/0 for booleans as well
        struct { // We need a struct for strings (with pointer and length)
            char* ptr;
            size_t len;
        } str;
        struct { // An object can be a list of more objects, recursive
            struct tfobj** ele;
            size_t len;
        } list;
    };
} tfobj;

typedef struct tfparser {
    char* prg; // The program to compile into a list
    char* p; // Next token to parse
} tfparser;

typedef struct tfctx {
    tfobj* stack;
} tfctx;


/* =========== ALLOCATION WRAPPERS =========== */
void* xmalloc(size_t size) {
    void* ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "Out of memory allocating %zu bytes\n", size);
        exit(1);
    }
    return ptr;
}

/* =========== OBJECT RELATED FUNCTIONS =========== */

tfobj* createObj(int type) {
    tfobj* o = xmalloc(sizeof(tfobj));
    o->type = type;
    o->refcount = 1;
    return o;
}

tfobj* createIntObj(int i) {
    tfobj* o = createObj(TFOBJ_TYPE_INT);
    o->i = i;
    return o;
}

tfobj* createBoolObj(int b) {
    tfobj* o = createObj(TFOBJ_TYPE_BOOL);
    o->i = b;
    return o;
}

tfobj* createStrObj(char* s, size_t len) {
    tfobj* o = createObj(TFOBJ_TYPE_STR);
    o->str.ptr = s;
    o->str.len = len;
    return o;
}

tfobj* createSymbolObj(char* s, size_t len) {
    tfobj* o = createObj(TFOBJ_TYPE_SYMBOL);
    o->str.ptr = s;
    o->str.len = len;
    return o;
}

tfobj* createListObj() {
    tfobj* o = createObj(TFOBJ_TYPE_LIST);
    o->list.ele = NULL;
    o->list.len = 0;
    return o;
}

/* =========== MAIN =========== */

int main(int argc /** The number of arguments, where the first one is always the name of the program */, char** argv /** The actual arguments */) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    return 0;
}