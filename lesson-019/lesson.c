#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
// #include <errno.h>

int main() {
    // open
    int fd = open("lesson-019/lesson.c", O_RDONLY);
    // printf("Error number is %d\n", errno);
    if (fd == -1) {
        perror("Unable to open file");
        return 1;
    }

    // read
    char buf[32];
    ssize_t nread;
    while (1) {
        nread = read(fd, buf, sizeof(buf));
        if (nread == -1) {
            perror("Reading file");
            return 1;
        }
        if (nread == 0) break;
        printf("%s", buf);
    }

    // close
    close(fd);
    return 0;
}