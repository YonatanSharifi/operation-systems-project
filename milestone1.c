#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Missing file\n");
        return 1;
    }

    int **matrix = read_file(argv[1]);
    if (!matrix) {
        perror("Open file failed");
        return 1;
    }

    dijkstra(matrix);
    int n = matrix[0][0];
    for (int i = 0; i <= n; i++)
        free(matrix[i]);
    free(matrix);

    return 0;
}
