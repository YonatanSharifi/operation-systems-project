#ifndef OSPROJECT_GRAPHICS_H
#define OSPROJECT_GRAPHICS_H

#include "raylib.h"

#define NODE_RADIUS 20.0f

void compute_circle_positions(int num_nodes, Vector2 center, float radius, Vector2 *out_positions);
void draw_arrow(Vector2 start, Vector2 end, int weight);
void draw_graph(int **matrix, int num_nodes, Vector2 *positions);

#endif
