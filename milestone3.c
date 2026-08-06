#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "raylib.h"
#include "dijkstra.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define NODE_RADIUS 20.0f

int* get_path_array(int **matrix, int *out_len) {
    int n = matrix[0][0];
    int start = matrix[0][1];
    int end = matrix[0][2];

    int *distance = malloc(n * sizeof(int));
    int *prev = malloc(n * sizeof(int));
    int *visited = malloc(n * sizeof(int));

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

        if (u == -1 || distance[u] == INT_MAX) break;
        visited[u] = 1;

        for (int v = 0; v < n; v++) {
            int w = matrix[u + 1][v];
            if (w > 0 && visited[v] == 0 && distance[u] + w < distance[v]) {
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

    int temp_path[100];
    int len = 0;
    for (int curr = end; curr != -1; curr = prev[curr]) {
        temp_path[len++] = curr;
    }

    int *final_path = malloc(len * sizeof(int));
    for (int i = 0; i < len; i++) {
        final_path[i] = temp_path[len - 1 - i];
    }
    *out_len = len;

    free(distance); free(prev); free(visited);
    return final_path;
}

void DrawArrow(Vector2 start, Vector2 end, int weight) {
    float dx = end.x - start.x;
    float dy = end.y - start.y;
    float length = sqrt(dx*dx + dy*dy);
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

typedef enum { STATE_IDLE, STATE_MOVING, STATE_WAITING, STATE_FINISHED } AnimState;

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
    int path_len = 0;
    int *path = get_path_array(matrix, &path_len);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OS Project - Milestone 3");
    SetTargetFPS(60);

    Vector2 *positions = malloc(num_nodes * sizeof(Vector2));
    Vector2 center = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    float radius = 220.0f;
    for (int i = 0; i < num_nodes; i++) {
        float angle = i * (2.0f * PI / num_nodes);
        positions[i].x = center.x + radius * cosf(angle);
        positions[i].y = center.y + radius * sinf(angle);
    }

    bool is_playing = false;
    AnimState state = STATE_IDLE;
    int current_path_idx = 0;
    float time_in_state = 0.0f;

    Rectangle btnBounds = { 10, 10, 100, 40 };

    while (!WindowShouldClose()) {
        Vector2 mousePoint = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePoint, btnBounds)) {
            if (state != STATE_FINISHED && path_len > 1) {
                is_playing = !is_playing;
                if (is_playing && state == STATE_IDLE) {
                    state = STATE_MOVING;
                }
            }
        }

        Vector2 entity_pos = { -100, -100 };
        if (path_len > 0) {
            if (state == STATE_IDLE) {
                entity_pos = positions[path[0]];
            } else if (state == STATE_FINISHED) {
                entity_pos = positions[path[path_len - 1]];
            } else {
                if (is_playing) {
                    time_in_state += GetFrameTime();
                }

                if (state == STATE_MOVING) {
                    int u = path[current_path_idx];
                    int v = path[current_path_idx + 1];
                    int weight = matrix[u + 1][v];

                    int current_jump = (int)(time_in_state / 0.3f);
                    if (current_jump >= weight) {
                        current_path_idx++;
                        time_in_state = 0.0f;
                        if (current_path_idx >= path_len - 1) {
                            state = STATE_FINISHED;
                            is_playing = false;
                        } else {
                            state = STATE_WAITING;
                        }
                    } else {
                        float fraction = (float)current_jump / weight;
                        entity_pos.x = positions[u].x + (positions[v].x - positions[u].x) * fraction;
                        entity_pos.y = positions[u].y + (positions[v].y - positions[u].y) * fraction;
                    }
                } else if (state == STATE_WAITING) {
                    entity_pos = positions[path[current_path_idx]];
                    if (is_playing && time_in_state >= 1.0f) {
                        state = STATE_MOVING;
                        time_in_state = 0.0f;
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int u = 0; u < num_nodes; u++) {
            for (int v = 0; v < num_nodes; v++) {
                int weight = matrix[u + 1][v];
                if (weight > 0) DrawArrow(positions[u], positions[v], weight);
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

        DrawRectangleRec(btnBounds, is_playing ? RED : GREEN);
        DrawText(is_playing ? "STOP" : "PLAY", 35, 20, 20, WHITE);

        if (path_len > 0) {
            DrawCircleV(entity_pos, 10.0f, ORANGE);
        }

        if (state == STATE_FINISHED) {
            DrawText("Destination Reached!", SCREEN_WIDTH / 2 - 150, 20, 30, DARKGREEN);
        } else if (path_len == 0) {
            DrawText("No path found", SCREEN_WIDTH / 2 - 100, 20, 30, RED);
        }

        EndDrawing();
    }

    CloseWindow();
    free(positions);
    if (path) free(path);
    for (int i = 0; i <= num_nodes; i++) free(matrix[i]);
    free(matrix);
    return 0;
}