//
// Created by student on 03/09/2026.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "travelers.h"

static char *read_stripped_content(const char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (!file) return NULL;

    size_t cap = 4096, len = 0;
    char *buffer = malloc(cap);
    buffer[0] = '\0';

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        char *p = line;
        while (*p == ' ' || *p == '\t') p++;
        if (*p == '#' || *p == '\n' || *p == '\0') continue;

        size_t line_len = strlen(p);
        if (len + line_len + 2 > cap) {
            cap = (len + line_len + 2) * 2;
            buffer = realloc(buffer, cap);
        }
        memcpy(buffer + len, p, line_len);
        len += line_len;
        buffer[len++] = ' ';
        buffer[len] = '\0';
    }

    fclose(file);
    return buffer;
}

int **read_multi_traveler_file(const char *file_name, TravelerSpec **out_travelers, int *out_num_travelers) {
    char *content = read_stripped_content(file_name);
    if (!content) return NULL;

    char *tok = strtok(content, " \t\n");
    if (!tok) { free(content); return NULL; }
    int num_nodes = atoi(tok);
    tok = strtok(NULL, " \t\n");
    int num_edges = tok ? atoi(tok) : 0;

    if (num_nodes < 0 || num_edges < 0) {
        printf("Negative numbers are not allowed\n");
        free(content);
        exit(1);
    }

    int **matrix = malloc((num_nodes + 1) * sizeof(int *));
    matrix[0] = malloc(3 * sizeof(int));
    matrix[0][0] = num_nodes;
    matrix[0][1] = 0;
    matrix[0][2] = 0;
    for (int i = 1; i <= num_nodes; i++) {
        matrix[i] = malloc(num_nodes * sizeof(int));
        for (int j = 0; j < num_nodes; j++) matrix[i][j] = 0;
    }

    for (int i = 0; i < num_edges; i++) {
        tok = strtok(NULL, " \t\n"); int from = tok ? atoi(tok) : 0;
        tok = strtok(NULL, " \t\n"); int to = tok ? atoi(tok) : 0;
        tok = strtok(NULL, " \t\n"); int weight = tok ? atoi(tok) : 0;
        if (from < 0 || to < 0 || weight < 0) {
            printf("Negative numbers are not allowed\n");
            free(content);
            exit(1);
        }
        matrix[from + 1][to] = weight;
    }

    tok = strtok(NULL, " \t\n");
    int num_travelers = tok ? atoi(tok) : 0;
    if (num_travelers < 0) {
        printf("Negative numbers are not allowed\n");
        free(content);
        exit(1);
    }

    TravelerSpec *travelers = malloc((num_travelers > 0 ? num_travelers : 1) * sizeof(TravelerSpec));
    for (int i = 0; i < num_travelers; i++) {
        tok = strtok(NULL, " \t\n"); travelers[i].source = tok ? atoi(tok) : 0;
        tok = strtok(NULL, " \t\n"); travelers[i].dest = tok ? atoi(tok) : 0;
    }

    free(content);
    *out_travelers = travelers;
    *out_num_travelers = num_travelers;
    return matrix;
}
