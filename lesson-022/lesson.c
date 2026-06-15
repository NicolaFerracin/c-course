#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


int compare(const void* a, const void* b) {
    const int* ap = a;
    const int* bp = b;
    if (*ap > *bp) return 1;
    if (*ap < *bp) return -1;
    return 0;
}

int main() {
    int arr[10];

    for (int i = 0; i < 10; i++) arr[i] = rand() & 15;

    arr[2] = INT_MAX;
    arr[5] = INT_MIN;

    qsort(arr, 10, sizeof(int), compare);

    for (int i = 0; i < 10; i++) printf("%d ", arr[i]);

    return 0;
}