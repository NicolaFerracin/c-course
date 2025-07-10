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
