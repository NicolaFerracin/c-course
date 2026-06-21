#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

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

/* =========== LIST OBJECT =========== */

/** Add new element at the end of the list "l" */
/** It is up to the caller to increment the reference count of the element added to the list. */
void listPush(tfobj* l, tfobj* ele) {
    l->list.ele = realloc(l->list.ele, sizeof(tfobj*) * (l->list.len + 1));
    l->list.ele[l->list.len] = ele;
    l->list.len++;
}

/* =========== TURN PROGRAM INTO TOY FORTH LIST =========== */
void parseSpaces(tfparser* parser) {
    while (isspace(parser->p[0])) parser->p++;
}

#define MAX_INT_LEN 128
tfobj* parseInt(tfparser* parser) {
    char buf[MAX_INT_LEN];
    char* start = parser->p;
    char* end;

    // Parse each characted as long as it's a number
    if (parser->p[0] == '-') parser->p++;
    while (parser->p[0] && isdigit(parser->p[0])) parser->p++;
    // We now where the number started, we now know where it ends
    end = parser->p;
    int numlen = end - start;
    if (numlen > MAX_INT_LEN) return NULL;

    // We can do memcpy to get the full number
    memcpy(buf, start, numlen);
    buf[numlen] = 0; // Add terminator

    tfobj* o = createIntObj(atoi(buf));
    return o;
}

tfobj* compile(char* prg) {
    tfparser parser;
    parser.prg = prg;
    parser.p = prg;

    tfobj* parsed = createListObj();

    while (parser.p) {
        tfobj* o;
        char* token_start = parser.p;

        parseSpaces(&parser);
        if (parser.p[0] == 0) break; // End of program reached

        if (isdigit(parser.p[0]) || parser.p[0] == '-') {
            o = parseInt(&parser);
        }
        else {
            o = NULL;
        }

        // Check if the current token produced a parsing error
        if (o == NULL) {
            // FIXME: Release parsed
            printf("Syntax error near: %32s ...\n", token_start);
            return NULL;
        }
        else {
            // If all went well, add the new object to the parsed list we have so far
            listPush(parsed, o);
        }
    }

    return parsed;
}

/* =========== EXECUTE THE PROGRAM =========== */

void exec(tfobj* prg) {
    printf("[");
    for (size_t j = 0; j < prg->list.len; j++) {
        tfobj* o = prg->list.ele[j];
        switch (o->type) {
        case TFOBJ_TYPE_INT:
            printf("%d ", o->i);
            break;
        default:
            printf("?");
            break;
        }
    }

    printf("]\n");
}


/* =========== MAIN =========== */

int main(int argc /** The number of arguments, where the first one is always the name of the program */, char** argv /** The actual arguments */) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE* fp = fopen(argv[1], "r");               // Open
    if (fp == NULL) {                             // Null file handling
        perror("Opening Toy Forth Program");
        return 1;
    }
    fseek(fp, 0, SEEK_END);                       // Go to end
    long file_size = ftell(fp);                   // Check size
    fseek(fp, 0, SEEK_SET);                       // Move cursor back to the start of the file
    char* prgtext = xmalloc(file_size + 1);       // Allocate memory to read the file + 1 byte for the null terminator
    fread(prgtext, file_size, 1, fp);             // Read file into prgtext
    prgtext[file_size] = 0;                       // Add null terminator
    fclose(fp);                                   // Close

    tfobj* prg = compile(prgtext);
    exec(prg);

    return 0;
}