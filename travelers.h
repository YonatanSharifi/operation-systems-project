#ifndef OSPROJECT_TRAVELERS_H
#define OSPROJECT_TRAVELERS_H

typedef struct {
    int source;
    int dest;
} TravelerSpec;

int **read_multi_traveler_file(const char *file_name, TravelerSpec **out_travelers, int *out_num_travelers);

#endif
