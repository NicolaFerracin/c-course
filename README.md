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
