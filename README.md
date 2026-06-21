# C Course By Salvatore Sanfilippo

https://www.youtube.com/watch?v=HjXBXBgfKyk&list=PLrEMgOSrS_3cFJpM2gdw8EGFyRBZOyAKY&index=9

## Lesson 1

- C is a compiled language
- `cc file.h` will compile the file to an executable
- `-S` flag will also output the assembly file
- `-O2` will use the O2 optimization, which will try to optimize the code
  - you can compare the assembly of the `hello world` assembly between no optimization and `-O2` optimization, and you'll notice that using the `O2`, will replace the `printf` with `puts` which is much faster in execution
  - using different optimizations WILL NOT change the program behaviour, just its implementation
  - optimizations are done by the compiler, so it's not a language feature, it's a compiler feature
- `#include` just gets replaced with the included file

  ```c
  // hello.c
  printf('Hello World\n');

  // main.c
  #include <stdio.h>

  int main() {
      #include "hello.c"
      return 0
  }
  ```

- we use `#include <stdio.h>` to import the `printf` function
  - we could alternatively include the function prototype in our file (no function body, just return type and arguments definition)
  - you can find the `printf` prototype definition with `man 3 printf`

## Lesson 2

```c
int main() {
    printf("Hello, World!\n");
    return 0;
}
```

- `int` is the return type
- `main` is the function name
- `()` means the `main` function does not take any argument
  - we could alternatively write it as `int main(void)`
- `printf` accepts a variable number of arguments
  - if we let `printf` know it should expect x arguments but we don't provide them, it will cause a warning `printf("Should expect 2 numbers %d, %d", 10)`
  - the program will still compile but there is no guaranteed behaviour: the program could crash, the second argument could be defaulted to 0 or anything else
- `main` should return `0` when successful

## Lesson 3

- defininig a variable within a function, makes it local
- defininig it outside, makes it global
- defininig it as static within a function, makes it work like a global variable
- in C variables are passed by values, not by reference
  ```c
  int main() {
    int a = 0;
    increment(a); // 1
    print("a: %d", a); // prints 0
  }
  ```
- with `printf` and in other functions, C does not handle all possible variable types. It promotes many of the types to the superst (i.e. `char` and `short` to `int`, `float` to `double`), meaning that subsets get converted to their superset and this way the functions need to handle a smaller set of types.

## Lesson 4

- types in C, as opposed to most other strictly typed languages, do not explicitely define how big the value ranges are. This is because C was created to be ran on all and any device, so it was never included in the specs what the value ranges shoud be
  - on `int` you can use `sizeof` to get the bytes size: `int x = 0; printf("int is %lu bytes\n", sizeof(x))`
  - to get the actual values you can include `limits`

  ```c
  #include <limits.h>

  int main() {
    int x = 0;
    printf("int min: %d, int max: %d\n", INT_MIN, INT_MAX);
  }
  ```

  ```c
  char c = 1; // 8 bit, 1 byte
  short s = 2000; // 16 bit, 8 2 byte
  int x = 5; // 32 bit, 4 bytes
  long l = 10; // mostly the same as the processor
  ```

- interesting exercise about recreating the `clear` function from the hexdump:
  - `clear | hexdump -C` to get the hexadecimal representation of the `clear` instruction
  - send that to `printf` to achieve the same `printf("\x1b\x5b\x33\x4a\x1b\x5b\x48\x1b\x5b\x32\x4a");`

## Lesson 5

- Use `-Wall -W -O2` when compiling to get detailed warnings and messages
- `char c = 'A'` using `''` turns A into the respective ASCII decimal representation. In C everything is a number
- strings are `int` arrays, made of single 1-byte `char`
- `0` is used as a `null terminator` to mark the string as complete
  - if you terminate a literal string array with something else than 0, the program will print gibberish

    ```c
    char str[6] = {'h', 'e', 'l', 'l', 'o', 0};
    printf("%s\n", str);
    return 0;

    // which means we can print strings char by char
    int i = 0;
    while (str[i] != 0) {
        printf("%c", str[i]);
        i++;
    }
    printf("\n");
    ```

- finally you define strings as `char str[5] = "Hello";`

## Lesson 6

- you define blocks wherever you want
  ```c
  int main() {
    int i = 8;
    {
      int i = 5;
      printf("%d", i); // i is 5
    }
    printf("%d", i); // i is 8
  }
  ```
- you can use `goto;again`
  - you can replicate a behaviour similar to basic, as `again` is just a label
  ```c
  int main() {
    l10: printf("infinite loop\n");
    l20: goto l10;
    return 0;
  }
  ```
- combininig `if` with `goto` you can achieve anything
- you can create `while` loops with `if` only, by using recursion
  - ALERT: using recursion consumes a lot of memory as each recursive call adds to the stack
  - you can use this to see that `start` and `end` keep changing address `printf("start: %p; end: %p\n", &start, &end);`

## Lesson 7

- building upon the recursion from last lesson, we can easily turn recursive code into iterative code by using `goto` and freeing a lot of memory

  ```c
  void count(int start, int end) {
  iterate: // add label
    if (start > end) return;
    printf("%d\n", start);
    // count(start + 1, end); gets removed
    start += 1; // replace start + 1
    printf("start: %p; end: %p\n", &start, &end);
    goto iterate; // start from the top
  }
  ```

  - checking the printf you can see `start` and `end` are always assigned to the same address: less memory usage!

- `goto` is useful but you usually want to leverage higher level paradigms (while, for,...)
- for loops are syntactic sugar around while loops
- you can strip away stuff from the for loop definition until you are left with a `while(1)`, which is the infinite loop in C

  ```c
  for (int i = 0; i < 10; i++) {
    printf("%d\n", i);
  }

  int i = 0;
  for (; i < 10; i++) {
    printf("%d\n", i);
  }

  int i = 0;
  for (;; i++) {
    if (i >= 10) break;
    printf("%d\n", i);
  }

  int i = 0;
  for (;;) {
    if (i >= 10) break;
    printf("%d\n", i);
    i++;
  }

  int i = 0;
  while (1) {
    if (i >= 10) break;
    printf("%d\n", i);
    i++;
  }
  ```

- C does not handle true or false, only 0 or 1, only integers
- you can't define variables within a `switch` statement
  - but remember we can create blocks anywhere!

  ```c
  int i = 0;
  switch (int) {
    case 5:
      int x = 10; // won't work
      break;
    case 7:
    {
      int x = 10 // DOES WORK!
      break;
    }
  }
  ```

## Lesson 9

- initializing a pointer to `0`, means you are using the null pointer, which is the same as `NULL`, a special address where nothing is stored
- when definining a pointer, you need to set the type of the value stored in the pointer

  ```c
  int x = 5;
  int *y = &x; // because y points to an address where an int is stored
  ```

## Lesson 10

More about pointers

- once you have a pointer to the address where a variable is stored, you can access the variable value with `p[0] = <new-value>`, which is the equal to `*p = <new-value>`
- `p[0]` or `*p` is called "dereferencing"
- you can find the pointer to a pointer by doing `int **z = &y`, where `int *y = &x` and `int x = 5`
- pointers are like long: they usually are the same size as the CPU address width (4 bytes on 32-bit systems, 8 bytes on 64-bit)
- you can do casting with `(int)sizeof(x)` (as the size is a long but we are printing an int)
- when defining strings you use arrays of char. The name of the array can be both used as a pointer and as a reference to the string:

  ```c
  char hello[] = "Hello world!";

  // hello is a pointer itself, no need to add & in front
  printf("String stored at %p with value %s\n", hello, hello);

  // because of the above we can also store it in a variable like this. NOTE we are not doing &hello
  char *p = hello;
  ```

- the pointer to a string points to the address of the first character. I can use dereferencing to get any char from the string

  ```c
  printf("%c %c %c\n", p[0], p[1], p[2]);         // array-style
  printf("%c %c %c\n", *p, *(p+1), *(p+2));       // pointer arithmetic
  ```

- I can also define a pointer with a different type. Doing `char *p` would create a pointer to a single char byte. But I could also do `short *s` which would point to the first 2 bytes

  ```c
  char hello[] = "Hello world!";
  char *p = hello;
  printf("1 byte pointer, %c\n", *p); // H

  short *s = (short*)hello; // using casting to short* to avoid warnings
  printf("2 bytes pointer, %d\n", *s); // 25928 - contrary to %c, with the short pointer, we get the actual integer stored in the 2 bytes that we are pointing to
  // 'H' is 72 and 'e' is 101 => 72 * 1 + 101 * 256 (1 byte = 256 bit) => 25928
  // we multiply 256 because we need to shift once (1 << 8)

  // another cool thing: incrementing the pointer, multiplies the increment by the base size
  p++; // will point to 'e'
  s++; // will not point to 'el' but will point to 'll'
  printf("1 byte pointer, %c\n", *p); // e
  printf("2 bytes pointer, %d\n", *s); // 27756 => l = 108 => 108 + 108*256 => 27756!
  ```

## Lesson 11

- we saw how string arrays have a final 0 byte, which is used to determine the string end (aka zero-terminated strings)

  ```c
  int main() {
    char str[] = "Hello";
    char* p = str;
    while (*p) putchar(*p++);
    printf("\n");

    return 0;
  }
  ```

- and you can put a 0 byte in the middle of the string, it stops prematurely

  ```c
   int main() {
    char str[] = "Hel\0lo";
    char* p = str;
    while (*p) putchar(*p++);
    printf("\n");

    return 0;
  }
  ```

- to being able to print the full string contents, even the 0 byte in the middle, we can use a length-prefixed string:

  ```c
  int main( {
    // we prefix the string with its own length (excluding the prefix itself BUT including the 0 byte in the middle)
    char str[] = "\6Hel\0lo";
      char* p = str;
      int len = *p++;
      printf("String length: %d\n", len);
      for (int i = 0; i < len; i++) putchar(p[i]);
      printf("\n");
  })
  ```

## Lesson 12

We saw the existence of length-prefixed strings and in this lesson we are exploring creating a super small library to create said strings in C.

The lesson is mostly code and touches on:

- creating a different way to handle strings
- limitations of functions returning pointers
- using `malloc`

Refer to the `lesson-012/lesson.c` file for the full commented code.

## Lesson 13

- From the previous lesson, we added a `lps_getc` function to get the string embedded within the length-prefixed string (lps)
  - Example:
    ```
    lps = |L|string (where L is the byte holding the length of the following string)
    c_str = string
    ```
- `lps_getc` helps with interoperability, so that we can use our LPS with the standard C strings when necessary (i.e. third-party libs, other C helpers...)
- we can do even better, when creating a new LPS, by returning the pointer to the string instead of the pointer to the beginning of the length header
- we could be passing a reference to the string several times in our program, so executing a the malloc free might render said references unusable
  - it would be cool if we could keep track of the active references as a second header, after the length header, so that we have an internal state that frees up the memory only when there are no active references left
  - keeping track of active references is known as "reference counting". As said, we could add another header and increase/decrease the counter every time we assign/remove a reference, but that is annoying to keep track of. What if we had a way to handle that more naturally?
  - segway into lesson 14 and C structs!
  ```c
  struct lps {
    uint32_t len;
    uint32_t refcount;
    char str[0]
  }
  ```

## Lesson 14

- Structs are the only aggregated data type in C. Eveything else is always and only numbers.

Refer to the `lesson-014/lesson.c` file for the full commented code.

## Lesson 15

Implement `tac` in C. It works exactly like `cat`, but printing lines in reversed order from last to first. The program shall make use of `structs` and linked lists.

Refer to the `lesson-015/lesson.c` file for the full commented code.

## Lesson 16

Going back to the end of lesson 13, where we wanted to add a second prefix to keep track of the active references, and we dived into structs. We can now implement the `lps` struct (length-prefixed string).

- when an interface uses reference counting, the function creating the object usually returns the object with the ref count set to 1 already
- conventionally we use a `release` and a `retain` function to increase/decrease the ref count
- we can add the concept of magic number set to a special value, which would help us validating strings. This is because we always operate with memory addresses, so once the memory allocated to a lps instance has been freed, we could still try to access it via release/retaing, but that would give us garbage values. Adding a validation check by using the magic number, prevents this from happening

Refer to the `lesson-016/lesson.c` file for the full commented code.

## Lesson 17

Refer to the `lesson-017/lesson.c` file for the full commented code.

## Lesson 18

`typedef` introduction:

- useful to alias types

  ```c
  typedef struct {
    int n;
    int d;
  } fract;

  fract f; // instead of 'struct fract f'
  ```

- allows to create opaque types, hiding implementation details
  ```c
  // FILE is a typedef: we don't see its internals
  FILE *fp = fopen("filepath.c", "r");
  fclose(fp)
  ```
- the other feature is encapsulation and abstraction, detailed in future lessons

## Lesson 19

- all (most?) C compilers come with a C-library, parts of which are just wrappers around system calls
- many unix systems are based on posix (solaris, linux, macOS, ...) and all share the same system calls with some additional system-based specifics
- C programs run inside the user-space. User-space does not have direct access to kernel-space. Opening a file is not done by the program itself but it's done via a system call where the program asks the system to open a file for them, in the kernel space. As the program does not have a way to represent and handle a document, the system simply returns a file descriptor (a number, an ID) so that the program can keep referencing the same file as needed

In this lesson we are replacing using the fopen functions from the stdio C-library, with system calls.

- We use `open` (from `fcntl.h`) as a system call to open a file
- We use `close` (from `unistd.h`) as another system call to close a file
- We log potential errors with `stdio.h` `perror`. Interestingly, `perror` also logs the reason of the error: "No such file or directory". How can it do that?
- You can see that importing `errno.h` you can print the int representing the last occurred error with `printf("Error number is %d\n", errno);`
- If you print `ENOENT` from the list of possible `open` errors, you see they match
- This means that errors are available on the thread and they can be interpreted based on the different meanings they have within a library

## Lesson 20

Exploring why and when to use system calls insted of libc calls:
Let's walk through a scenario.

The following will sleep before printing.

```c
printf("Hello world");
sleep(5);
```

This is becuase what the libc `printf` call does, is to put everything into a buffer and the buffer gets actually printed only after flushing or after a new line.

This is because system calls are expensive and libc calls work around that cost by using the buffer.

Direct system calls can avoid libc buffering overhead in some cases and result in faster execution, but they can also be a pitfall if used incorrectly. Many small system calls are usually slower than buffered libc I/O, so performance depends on the
access pattern, buffering strategy, and syscall count.
Adding the flush, the "Hello World" will get printed before sleeping.

```c
printf("Hello world");
fflush(stdout);
sleep(5);
```

This is how it would look like with the `write` system call, where the sleeping also happens last (but it's potentially more expensive if not part of a performant and well crafted program).

```c
char* mystr = "Hello World";
write(STDOUT_FILENO, mystr, strlen(mystr));
sleep(5);
```

In the previous lesson, we talked about how C programs run in user space and cannot directly access kernel-managed resources like open files. The `mmap` system call lets a program map a file into its own virtual address space, so the file contents can be accessed through ordinary memory reads instead of repeated `read` calls.

## Lesson 21

### Union

Looking into `union` starting from an example:

```c
struct foo {
    int i;
    unsigned char a[4];
};
struct foo f = { 10, { "abcd" } };
printf("%d [%d %d %d %d]\n", f.i, f.a[0], f.a[1], f.a[2], f.a[3]);
```

As expected we print `10 [97 98 99 100]`.

But if defined `i` and `a` within an union, they share the same starting address:

```c
struct ufoo {
    union {
        int i;
        unsigned char a[4];
    };
};
struct ufoo uf = { 10 };
printf("%d [%d %d %d %d]\n", uf.i, uf.a[0], uf.a[1], uf.a[2], uf.a[3]);
```

As `i` and `a` share the same memory address, we print `10 [10 0 0 0]`.

One of the most common reasons to use unions is showcased in this example:
let's say we have a struct that holds a value but the value can be of different types.
Instead of allocating a bigger memory address to support all possible types, we can use union to allocate a large enough space to hold the largest member, instead of the sum of all the members.

```c
struct typevalue {
  int type;
  union {
    double num;
    char str[10];
    int opcode;
  };
};
```

When instantiating a new `typevalue` instance, instead of reserving space for a `double`, a `char` array and an `int`, we use the union so that the 3 different types share the same starting memory address.

### Bitfield

In C we can say how much bits to allocate for a given variable.

```c
struct bar {
    unsigned char a : 4;
    unsigned char b : 4;
    unsigned char c : 8;
};
```

`a` and `b` can hold 4-bits numbers (2^4 - 1 => 0..15) and `c` can go up to 8-bits (1-byte => 255).

Given that we have a pre-allocated space, if we provide too big of a number, it will wrap:

```c
struct bar b;
b.a = 17;
printf("%d", b.a) // will print 1, which is (17 % 16 = 1)
```

This wrap around is the behaviour for `unsigned` values, but for signed values the behaviour is undefined.

## Lesson 22

Lesson about function pointers.. Nothing new coming from high-level languages, it's mainly about being able to declare a pointer to a function which allows to passing a function around.

Like arrays, function names can be used as pointers. A function pointer stores the address of executable code, and another function can call it later.

Simple example in the lesson-022 folder, showing how to pass a comparator function to sort, as you would do in `js` with `Array.sort()`.

## Lesson 23

Let's put it all together into something more complex. A code interpreter.

Let's use a Forth-like language (we'll call ToyForth), which is a stack-oriented language (`5 5 +` => 10).

The stack contains objects which get evaluated one at a time.

Let's start with the data types, by first defining the stack object:

```c
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
```

We then need a way to parse the actual program, passed from the CLI:

```c
typedef struct tfparser {
    char* prg; // The program to compile into a list
    char* p; // Next token to parse
} tfparser;

int main(int argc /** The number of arguments, where the first one is always the name of the program */, char** argv /** The actual arguments */) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    return 0;
}
```

We also need to define a data structure for the context holding the program. `tfctx` is the interpreter context. It groups together the runtime state of the ToyForth program, starting with the stack. This makes it easier to pass interpreter state between functions without passing many separate arguments.

```c
typedef struct tfctx {
    tfobj* stack;
} tfctx;
```

We can then create all the functions required to create stack objects of different types, by first defining a function to allocate an object and then to create the object for each type:

```c
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

tfobj* createListObj(void) {
    tfobj* o = createObj(TFOBJ_TYPE_LIST);
    o->list.ele = NULL;
    o->list.len = 0;
    return o;
}
```

Notice how we use `xmalloc`: it's a small wrapper around `malloc`. It centralizes out-of-memory handling, so the rest of the code can allocate memory without repeating the same `NULL` check every time.

```c
void* xmalloc(size_t size) {
    void* ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "Out of memory allocating %zu bytes\n", size);
        exit(1);
    }
    return ptr;
}
```

## Lesson 24

Continuation of the Forth-like language compiler started in Lesson 23.

Let's now define a very simple `.tf` program we can then read and interpret:

```
5 10 20
```

And let's read it from the main:

```c
FILE* fp = fopen(argv[1], "r");               // Open
if (fp == NULL) {                             // Null file handling
    perror("Opening Toy Forth Program");
    return 1;
}
fseek(fp, 0, SEEK_END);                       // Go to end
long file_size = ftell(fp);                   // Get file length
fseek(fp, 0, SEEK_SET);                       // Move cursor back to the start of the file
char* prgtext = xmalloc(file_size + 1);       // Allocate memory to read the file + 1 byte for the null terminator
fread(prgtext, file_size, 1, fp);             // Read file into prgtext
prgtext[file_size] = 0;                       // Add null terminator
fclose(fp);                                   // Close

printf("Program text: %s\n", prgtext);        // Print TF program
```

Now it becomes a bit annoying to copy/paste all the snippets as they are getting longer, but in a few words we:

- Read the program file into memory (the snippet just above here)
- Call `compile` on it, which takes care of identifying the program elements by parsing spaces and for now only parsing integers
- Create `parseInt` to parse integers whenever we identify one (either by seeing digit or a - sign)
  - We keep reading from the program file as long as we see digits
  - We keep track of the start and end of the number digit positions
  - We copy the token into a temporary null-terminated buffer and call `atoi` to convert that token to an integer. I.E: in `abc 1234 def` the number goes from position 4 to 7
- Once we parsed the integer (and later all the other types) we create an object representation of it and we add it at the end of the list
- Once we compiled the whole program file we return the `tfobj` representing it
- We call `exec` on the compiled program, which for now just prints the elements
