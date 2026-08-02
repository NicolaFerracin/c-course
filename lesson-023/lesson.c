#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

/* =========== DATA STRUCTURES =========== */

#define TF_OK 0
#define TF_ERR 1

#define TFOBJ_TYPE_INT 0
#define TFOBJ_TYPE_STR 1
#define TFOBJ_TYPE_BOOL 2
#define TFOBJ_TYPE_LIST 3
#define TFOBJ_TYPE_SYMBOL 4
#define TFOBJ_TYPE_ALL 255 // Used by listPopType() and other functions

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


typedef struct tfctx tfctx;

/** Each entry represents a symbol and its associated function and logic. */
typedef struct FunctionTableEntry {
    char* name;
    int (*callback) (tfctx* ctx, char* name);
    tfobj* user_func;
} tffuncentry;

typedef struct FunctionTable {
    tffuncentry** func_table;
    size_t func_count;
} tffunctable;

typedef struct tfctx {
    tfobj* stack;
    tffunctable functable;
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

void* xrealloc(void* oldptr, size_t size) {
    void* ptr = realloc(oldptr, size);
    if (ptr == NULL) {
        fprintf(stderr, "Out of memory re-allocating %zu bytes\n", size);
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
    o->str.ptr = xmalloc(len + 1);
    o->str.len = len;
    memcpy(o->str.ptr, s, len);
    o->str.ptr[len] = 0;
    return o;
}

tfobj* createSymbolObj(char* s, size_t len) {
    tfobj* o = createObj(TFOBJ_TYPE_SYMBOL);
    o->str.ptr = xmalloc(len + 1);
    o->str.len = len;
    memcpy(o->str.ptr, s, len);
    o->str.ptr[len] = 0;
    return o;
}

tfobj* createListObj() {
    tfobj* o = createObj(TFOBJ_TYPE_LIST);
    o->list.ele = NULL;
    o->list.len = 0;
    return o;
}


void printObject(tfobj* o) {
    switch (o->type) {
    case TFOBJ_TYPE_INT:
        printf("%d", o->i);
        break;
    case TFOBJ_TYPE_STR:
        printf("\"%s\"", o->str.ptr);
        break;
    case TFOBJ_TYPE_SYMBOL:
        printf("%s", o->str.ptr);
        break;
    case TFOBJ_TYPE_LIST:
        printf("[");
        for (size_t j = 0; j < o->list.len; j++) {
            tfobj* ele = o->list.ele[j];
            printObject(ele);
            if (j != o->list.len - 1) printf(" ");
        }
        printf("]");
        break;
    default:
        printf("?");
        break;
    }
}

void release(tfobj* o);

void freeObject(tfobj* o) {
    switch (o->type) {
    case TFOBJ_TYPE_LIST:
        for (size_t j = 0; j < o->list.len; j++) {
            tfobj* ele = o->list.ele[j];
            release(ele);
        }
        break;
    case TFOBJ_TYPE_SYMBOL:
    case TFOBJ_TYPE_STR:
        free(o->str.ptr);
        break;
    }
    free(o);
}

tfobj* retain(tfobj* o) {
    o->refcount++;
    return o;
}

void release(tfobj* o) {
    assert(o->refcount > 0);
    o->refcount--;
    if (o->refcount == 0) freeObject(o);
    return;
}

int isSameStringObject(tfobj* a, tfobj* b) {
    if (a->str.len != (b->str.len)) return 1;
    if (memcmp(a->str.ptr, b->str.ptr, a->str.len) == 0) return 0;
    return 1;
}
int isSameSymbolObject(tfobj* a, tfobj* b) {
    return isSameStringObject(a, b);
}

/* =========== LIST OBJECT =========== */

/** Add new element at the end of the list "l" */
/** It is up to the caller to increment the reference count of the element added to the list. */
void listPush(tfobj* l, tfobj* ele) {
    l->list.ele = xrealloc(l->list.ele, sizeof(tfobj*) * (l->list.len + 1));
    l->list.ele[l->list.len] = ele;
    l->list.len++;
}

tfobj* listPopType(tfctx* ctx, int type) {
    tfobj* stack = ctx->stack;
    if (stack->list.len == 0) return NULL;
    tfobj* to_pop = stack->list.ele[stack->list.len - 1];
    if (to_pop->type != type) return NULL;
    stack->list.len--;
    if (stack->list.len == 0) {
        free(stack->list.ele);
        stack->list.ele = NULL;
    }
    else {
        stack->list.ele = xrealloc(stack->list.ele, sizeof(tfobj*) * stack->list.len);
    }
    return to_pop;
}

tfobj* listPop(tfctx* ctx) {
    return listPopType(ctx, TFOBJ_TYPE_ALL);
}

/* =========== CONTEXT HANDLERS =========== */

int ctxCheckStackMinLen(tfctx* ctx, size_t minlen) {
    return (ctx->stack->list.len >= minlen) ? TF_OK : TF_ERR;
}

// Pop the top object in the interpreter main stack
tfobj* ctxStackPop(tfctx* ctx, int type) {
    return listPopType(ctx, type);
}

// Push the object on the interpreter main stack
void ctxStackPush(tfctx* ctx, tfobj* obj) {
    listPush(ctx->stack, obj);
}

/* =========== TURN PROGRAM INTO TOY FORTH LIST =========== */
void parseSpaces(tfparser* parser) {
    while (isspace(parser->p[0])) parser->p++;
}

#define MAX_INT_LEN 128
tfobj* parseInt(tfparser* parser) {
    char buf[MAX_INT_LEN];
    char* start = parser->p;

    // Parse each characted as long as it's a number
    if (parser->p[0] == '-') parser->p++;
    while (parser->p[0] && isdigit(parser->p[0])) parser->p++;
    // We now where the number started, we now know where it ends
    int numlen = parser->p - start;
    if (numlen > MAX_INT_LEN) return NULL;

    // We can do memcpy to get the full number
    memcpy(buf, start, numlen);
    buf[numlen] = 0; // Add terminator

    tfobj* o = createIntObj(atoi(buf));
    return o;
}

int is_symbol_character(int c) {
    char symchars[] = "+-*/%";
    if (c == 0) return 0;
    return isalpha(c) || strchr(symchars, c) != NULL;
}

tfobj* parseSymbol(tfparser* parser) {
    char* start = parser->p;
    while (is_symbol_character(parser->p[0])) parser->p++;
    int symlen = parser->p - start;
    return createSymbolObj(start, symlen);
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

        if (isdigit(parser.p[0]) || (parser.p[0] == '-' && isdigit(parser.p[1]))) {
            o = parseInt(&parser);
        }
        else if (is_symbol_character(parser.p[0])) {
            o = parseSymbol(&parser);
        }
        else {
            o = NULL;
        }

        // Check if the current token produced a parsing error
        if (o == NULL) {
            release(parsed);
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

/* =========== BASIC STANDARD LIBRARY =========== */

int basicMathFunction(tfctx* ctx, char* name) {
    if (ctxCheckStackMinLen(ctx, 2)) return TF_ERR;
    tfobj* b = ctxStackPop(ctx, TFOBJ_TYPE_INT);
    if (b == NULL) return TF_ERR;
    tfobj* a = ctxStackPop(ctx, TFOBJ_TYPE_INT);
    if (a == NULL) {
        ctxStackPush(ctx, b);
        return TF_ERR;
    }

    int result;
    switch (name[0]) {
    case '+': result = a->i + b->i; break;
    case '-': result = a->i - b->i; break;
    case '*': result = a->i * b->i; break;
    }
    release(a);
    release(b);

    ctxStackPush(ctx, createIntObj(result));
    return TF_OK;
}

/* =========== EXECUTION AND CONTEXT =========== */

// Resolve the function scanning the function table looking for the given function name.
tffuncentry* getFunctionByName(tfctx* ctx, char* name) {
    for (size_t j = 0; j < ctx->functable.func_count; j++) {
        tffuncentry* fe = ctx->functable.func_table[j];
        if (strcmp(fe->name, name) == 0) return fe;
    }
    return NULL;
}

tffuncentry* registerFunction(tfctx* ctx, char* name) {
    ctx->functable.func_table = xrealloc(ctx->functable.func_table, sizeof(tffuncentry*) * (ctx->functable.func_count + 1));
    tffuncentry* fe = xmalloc(sizeof(tffuncentry));
    ctx->functable.func_table[ctx->functable.func_count] = fe;
    ctx->functable.func_count++;
    fe->name = xmalloc(strlen(name) + 1);
    strcpy(fe->name, name);
    fe->callback = NULL;
    fe->user_func = NULL;
    return fe;
}

/** Register a new function with the given name in the function table within the context.
 * The function replaces existing functions with the same name with the new implementation;
*/
void registerCFunction(tfctx* ctx, char* name, int (*callback) (tfctx* ctx, char* name)) {
    tffuncentry* fe;
    fe = getFunctionByName(ctx, name);
    if (fe) {
        if (fe->user_func) {
            release(fe->user_func);
            fe->user_func = NULL;
        }
        fe->callback = callback;
    }
    else {
        fe = registerFunction(ctx, name);
        fe->callback = callback;
    }
}


tfctx* createCtx(void) {
    tfctx* ctx = xmalloc(sizeof(*ctx));
    ctx->stack = createListObj();
    ctx->functable.func_table = NULL;
    ctx->functable.func_count = 0;
    registerCFunction(ctx, "+", basicMathFunction);
    registerCFunction(ctx, "-", basicMathFunction);
    registerCFunction(ctx, "*", basicMathFunction);
    return ctx;
}


/** Try to resolve and call the function associated with the given symbol "word".
 * Returns 0 when the symbol "word" can be resolved to a function and was executed.
 * Returns 1 otherwise (on error).
 */
int callSymbol(tfctx* ctx, tfobj* word) {
    tffuncentry* fe = getFunctionByName(ctx, word->str.ptr);
    if (fe == NULL) return TF_ERR;
    if (fe->user_func) {
        // TODO
        return TF_ERR;
    }
    else {
        return fe->callback(ctx, fe->name);
    }
}

int exec(tfctx* ctx, tfobj* prg) {
    // Assert our program is a list of objects.
    assert(prg->type == TFOBJ_TYPE_LIST);

    // Process one item (word) in the list at a time.
    for (size_t j = 0; j < prg->list.len; j++) {
        tfobj* word = prg->list.ele[j];
        switch (word->type) {
        case TFOBJ_TYPE_SYMBOL:
            if (callSymbol(ctx, word) == TF_ERR) {
                printf("Run time error\n");
                return TF_ERR;
            }
            break;
        default:
            ctxStackPush(ctx, word);
            retain(word);
            break;
        }
    }
    return TF_OK;
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
    printf("Program content:\n");
    printObject(prg);
    printf("\n");
    printf("\n");

    tfctx* ctx = createCtx();
    printf("Context created.\n");

    exec(ctx, prg);
    printf("Stack content:\n");
    printObject(ctx->stack);
    printf("\n");

    return 0;
}
