
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "dijkstra.h"

int** read_file(const char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (!file) {
        return NULL;
    }

    int num_nodes, edges;
    fscanf(file, "%d %d", &num_nodes, &edges);
    if (num_nodes < 0 || edges < 0) {
        printf("Negative numbers are not allowed\n");
        fclose(file);
        exit(1);
    }

    int **matrix = malloc((num_nodes +1)*sizeof(int*));
    matrix[0] = malloc(3*sizeof(int));
    matrix[0][0] = num_nodes;
    matrix[0][1] = 0;
    matrix[0][2] = 0;
    for (int i = 1; i <= num_nodes; i++) {
        matrix[i] = malloc(num_nodes*sizeof(int));
        for (int j = 0; j < num_nodes; j++)
            matrix[i][j] = 0;
    }

    for (int i = 0; i < edges; i++) {
        int from, to, weight;
        fscanf(file, "%d %d %d", &from, &to, &weight);
        if (from < 0 || to < 0 || weight < 0) {
            printf("Negative numbers are not allowed\n");
            fclose(file);
            exit(1);
        }
        matrix[from + 1][to] = weight;
    }

    fscanf(file, "%d %d", &matrix[0][1], &matrix[0][2]);
    if (matrix[0][1] < 0 || matrix[0][2] < 0) {
        printf("Negative numbers are not allowed\n");
        fclose(file);
        exit(1);
    }
    fclose(file);
    return matrix;
}

void dijkstra(int **matrix) {
    int n     = matrix[0][0];
    int start = matrix[0][1];
    int end   = matrix[0][2];

    if (start == end) {
        printf("%d\n0\n", start);
        return;
    }

    int *distance = malloc(n*sizeof(int));
    int *prev = malloc(n*sizeof(int));
    int *visited = malloc(n*sizeof(int));

    for (int i = 0; i < n; i++) {
        distance[i] = INT_MAX;
        prev[i] = -1;
        visited[i] = 0;
    }
    distance[start] = 0;

    for (int count = 0; count < n; count++) {
        int u = -1;
        for (int i = 0; i < n; i++)
            if (visited[i] == 0 && (u == -1 || distance[i] < distance[u]))
                u = i;

        if (u == -1 || distance[u] == INT_MAX) {
            break;
        }
        visited[u] = 1;

        for (int v = 0; v < n; v++) {
            int w = matrix[u+1][v];
            if (w > 0 && visited[v] == 0 && distance[u] + w < distance[v]) {
                distance[v] = distance[u] + w;
                prev[v] = u;
            }
        }
    }

    if (distance[end] == INT_MAX) {
        printf("No path found\n");
        free(distance);
        free(prev);
        free(visited);
        return;
    }

    int *path = malloc(n*sizeof(int));
    int path_len = 0;
    for (int curr = end; curr != -1; curr = prev[curr])
        path[path_len++] = curr;

    printf("%d", path[path_len - 1]);
    for (int i = path_len - 2; i >= 0; i--)
        printf(" -> %d", path[i]);
    printf("\n%d\n", distance[end]);

    free(distance);
    free(prev);
    free(visited);
    free(path);
}

int* compute_path(int **matrix, int start, int end, int *out_len) {
    int n = matrix[0][0];
    int *distance = malloc(n * sizeof(int));
    int *prev = malloc(n * sizeof(int));
    int *visited = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) { distance[i] = INT_MAX; prev[i] = -1; visited[i] = 0; }
    distance[start] = 0;

    for (int count = 0; count < n; count++) {
        int u = -1;
        for (int i = 0; i < n; i++)
            if (!visited[i] && (u == -1 || distance[i] < distance[u])) u = i;
        if (u == -1 || distance[u] == INT_MAX) break;
        visited[u] = 1;
        for (int v = 0; v < n; v++) {
            int w = matrix[u + 1][v];
            if (w > 0 && !visited[v] && distance[u] + w < distance[v]) {
                distance[v] = distance[u] + w;
                prev[v] = u;
            }
        }
    }

    if (distance[end] == INT_MAX) {
        *out_len = 0;
        free(distance); free(prev); free(visited);
        return NULL;
    }

    int *temp_path = malloc(n * sizeof(int));
    int len = 0;
    for (int curr = end; curr != -1; curr = prev[curr]) temp_path[len++] = curr;

    int *final_path = malloc(len * sizeof(int));
    for (int i = 0; i < len; i++) final_path[i] = temp_path[len - 1 - i];

    *out_len = len;
    free(temp_path); free(distance); free(prev); free(visited);
    return final_path;
}