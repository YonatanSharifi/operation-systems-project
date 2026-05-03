#include <stdio.h>
#include "dijkstra.h"

int main(void) {
    int **matrix = read_file("exemple.txt");

    int num_nodes = matrix[0][0];
    int src       = matrix[0][1];
    int dest      = matrix[0][2];

    printf("Nodes: %d\n", num_nodes);
    printf("Source: %d  Destination: %d\n", src, dest);
    printf("\nAdjacency matrix:\n");
    printf("   ");
    for (int i = 0; i < num_nodes; i++)
        printf("%3d", i);
    printf("\n");
    for (int i = 0; i < num_nodes; i++) {
        printf("%3d", i);
        for (int j = 0; j < num_nodes; j++)
            printf("%3d", matrix[i + 1][j]);
        printf("\n");
    }

    return 0;
}
