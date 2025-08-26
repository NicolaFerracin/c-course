#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct file_line {
    char* line;
    struct file_line* prev_line;
};

int main(int args, char** argv) {
    // Print all arguments
    for (int i = 0; i < args; i++) {
        printf("%d: %s\n", i, argv[i]);
        /**
         * cc lesson-015/lesson.c -Wall -W -O2 && ./a.out b c d e f
           0: ./a.out
           1: b
           2: c
           3: d
           4: e
           5: f
         */
    }

    // Check arguments
    if (args != 2) {
        printf("Please, pass a single argument pointing to a file to read.\n");
        return 1;
    }

    // Check file
    FILE* fs = fopen(argv[1], "r");
    if (fs == NULL) {
        printf("File '%s' does not exist.\n", argv[1]);
        return 1;
    }

    char buf[1024];
    struct file_line* head = NULL;
    while (fgets(buf, sizeof(buf), fs) != NULL) {
        // initialize struct for current line
        struct file_line* curr_line = malloc(sizeof(*curr_line));

        // initialize char member that holds the actual line string
        size_t line_len = strlen(buf);
        curr_line->line = malloc(line_len + 1); // +1 for the null terminator

        // copy line char by char
        for (size_t c = 0; c <= line_len; c++) curr_line->line[c] = buf[c];

        // assign and move head
        curr_line->prev_line = head;
        head = curr_line;
    }

    fclose(fs);

    while (head) {
        struct file_line* to_free = head;

        printf("%s", head->line);
        head = head->prev_line;

        free(to_free->line);
        free(to_free);
    }
}