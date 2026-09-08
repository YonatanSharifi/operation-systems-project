#include <math.h>
#include <stdio.h>
#include "graphics.h"

void compute_circle_positions(int num_nodes, Vector2 center, float radius, Vector2 *out_positions) {
    for (int i = 0; i < num_nodes; i++) {
        float angle = i * (2.0f * PI / num_nodes);
        out_positions[i].x = center.x + radius * cosf(angle);
        out_positions[i].y = center.y + radius * sinf(angle);
    }
}

void draw_arrow(Vector2 start, Vector2 end, int weight) {
    float dx = end.x - start.x;
    float dy = end.y - start.y;
    float length = sqrtf(dx * dx + dy * dy);
    if (length == 0) return;
    dx /= length; dy /= length;

    Vector2 adjStart = { start.x + dx * NODE_RADIUS, start.y + dy * NODE_RADIUS };
    Vector2 adjEnd = { end.x - dx * NODE_RADIUS, end.y - dy * NODE_RADIUS };

    DrawLineEx(adjStart, adjEnd, 2.0f, DARKGRAY);

    float arrowSize = 15.0f;
    float angle = atan2f(dy, dx);
    Vector2 p1 = { adjEnd.x - arrowSize * cosf(angle - 0.5f), adjEnd.y - arrowSize * sinf(angle - 0.5f) };
    Vector2 p2 = { adjEnd.x - arrowSize * cosf(angle + 0.5f), adjEnd.y - arrowSize * sinf(angle + 0.5f) };

    DrawLineEx(adjEnd, p1, 2.0f, DARKGRAY);
    DrawLineEx(adjEnd, p2, 2.0f, DARKGRAY);

    Vector2 mid = { (adjStart.x + adjEnd.x) / 2, (adjStart.y + adjEnd.y) / 2 };
    char weightStr[16];
    sprintf(weightStr, "%d", weight);
    DrawText(weightStr, (int)mid.x, (int)mid.y - 15, 20, MAROON);
}

void draw_graph(int **matrix, int num_nodes, Vector2 *positions) {
    for (int u = 0; u < num_nodes; u++)
        for (int v = 0; v < num_nodes; v++)
            if (matrix[u + 1][v] > 0) draw_arrow(positions[u], positions[v], matrix[u + 1][v]);

    for (int i = 0; i < num_nodes; i++) {
        DrawCircleV(positions[i], NODE_RADIUS, SKYBLUE);
        DrawCircleLines((int)positions[i].x, (int)positions[i].y, NODE_RADIUS, DARKBLUE);
        char idStr[11];
        sprintf(idStr, "%d", i);
        int textWidth = MeasureText(idStr, 20);
        DrawText(idStr, (int)positions[i].x - textWidth / 2, (int)positions[i].y - 10, 20, BLACK);
    }
}
