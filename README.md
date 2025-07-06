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
