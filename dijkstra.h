#ifndef OSPROJECT_DIJKSTRA_H
#define OSPROJECT_DIJKSTRA_H

int** read_file(const char *file_name);
void dijkstra(int **matrix);
int* compute_path(int **matrix, int start, int end, int *out_len);

#endif
