#include <stdio.h>

struct fractstruct {
    int n;
    int d;
};

typedef struct {
    int n;
    int d;
} fracttype;

int main() {
    struct fractstruct fs;
    fs.n = 20;
    fs.d = 10;
    printf("%d/%d\n", fs.n, fs.d);

    fracttype ft;
    ft.n = 30;
    ft.d = 10;
    printf("%d/%d\n", ft.n, ft.d);

    FILE* fp = fopen("./lesson-018.c", "r");
    fclose(fp);

    return 0;
}
