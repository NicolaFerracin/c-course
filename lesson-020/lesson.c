#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>

int main() {
    // getting a file descriptor
    int fd = open("lesson-020/lesson.c", O_RDONLY);
    printf("Open file descriptor: %d\n", fd);
    
    // allocating the files to our memory space
    void* mem = mmap(NULL, 100, PROT_READ, MAP_FILE | MAP_SHARED, fd, 0);
    printf("File mapped at: %p\n", mem);

    // reading through the file from the program
    // notice how we are not using a system call or a libc call but a for loop instead
    // while accessing the characters stored at the given memory addresses!
    char* s = mem;
    for (int j = 0; j < 10; j++)
        printf("s[%d] = %c\n", j, s[j]);
    return 0;
}