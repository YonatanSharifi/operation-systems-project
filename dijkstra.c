#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"

int** read_file(char *file_name) {
    FILE *fp = fopen(file_name, "r");
    if (!fp) return NULL;

    int num_nodes, edges;
    fscanf(fp, "%d %d", &num_nodes, &edges);

    /* matrix[0] = {num_nodes, src, dest}, matrix[1..num_nodes] = adjacency rows */
    int **matrix = malloc((num_nodes + 1) * sizeof(int *));
    matrix[0] = malloc(3 * sizeof(int))
    ;
    matrix[0][0] = num_nodes;
    matrix[0][1] = 0;
    matrix[0][2] = 0;
    for (int i = 1; i <= num_nodes; i++) {
        matrix[i] = malloc(num_nodes * sizeof(int));
        for (int j = 0; j < num_nodes; j++)
            matrix[i][j] = 0;
    }

    for (int i = 0; i < edges; i++) {
        int from, to, weight;
        fscanf(fp, "%d %d %d", &from, &to, &weight);
        matrix[from + 1][to] = weight;
    }

    fscanf(fp, "%d %d", &matrix[0][1], &matrix[0][2]);

    fclose(fp);
    return matrix;
}