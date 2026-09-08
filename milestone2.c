#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"
#include "dijkstra.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define NODE_RADIUS 20.0f
void DrawArrow(Vector2 start, Vector2 end, int weight) {
    float dx = end.x - start.x;
    float dy = end.y - start.y;
    float length = sqrt(dx*dx + dy*dy);
    if (length == 0) return;
    dx /= length;
    dy /= length;
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

    int num_nodes = matrix[0][0];
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OS Project - Milestone 2");
    SetTargetFPS(60);
    Vector2 *positions = malloc(num_nodes * sizeof(Vector2));
    Vector2 center = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    float radius = 220.0f;
    for (int i = 0; i < num_nodes; i++) {
        float angle = i * (2.0f * PI / num_nodes);
        positions[i].x = center.x + radius * cosf(angle);
        positions[i].y = center.y + radius * sinf(angle);
    }
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        for (int u = 0; u < num_nodes; u++) {
            for (int v = 0; v < num_nodes; v++) {
                int weight = matrix[u + 1][v];
                if (weight > 0) {
                    DrawArrow(positions[u], positions[v], weight);
                }
            }
        }
        for (int i = 0; i < num_nodes; i++) {
            DrawCircleV(positions[i], NODE_RADIUS, SKYBLUE);
            DrawCircleLines((int)positions[i].x, (int)positions[i].y, NODE_RADIUS, DARKBLUE);

            char idStr[11];
            sprintf(idStr, "%d", i);
            int textWidth = MeasureText(idStr, 20);
            DrawText(idStr, (int)positions[i].x - textWidth / 2, (int)positions[i].y - 10, 20, BLACK);
        }
        EndDrawing();
    }
    CloseWindow();
    free(positions);
    for (int i = 0; i <= num_nodes; i++) {
        free(matrix[i]);
    }
    free(matrix);
    return 0;
}
