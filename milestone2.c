#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"
#include "dijkstra.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Missing file\n");
        return 1;
    }

    int **matrix = read_file(argv[1]);
    if (!matrix) {
        printf("Failed to read graph data.\n");
        return 1;
    }

    int numNodes = matrix[0][0];

    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Graph Visualization - Milestone 2");
    SetTargetFPS(60);

    Vector2 *nodePositions = (Vector2 *)malloc(numNodes * sizeof(Vector2));
    float radius = 220.0f;
    Vector2 center = { screenWidth / 2.0f, screenHeight / 2.0f };
    float angleStep = (2 * PI) / numNodes;

    for (int i = 0; i < numNodes; i++) {
        nodePositions[i].x = center.x + radius * cos(i * angleStep);
        nodePositions[i].y = center.y + radius * sin(i * angleStep);
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int u = 0; u < numNodes; u++) {
            for (int v = 0; v < numNodes; v++) {
                int weight = matrix[u+1][v];
                if (weight > 0) {
                    Vector2 startPos = nodePositions[u];
                    Vector2 endPos = nodePositions[v];

                    DrawLineEx(startPos, endPos, 2.0f, DARKGRAY);

                    Vector2 midPoint = { (startPos.x + endPos.x) / 2.0f, (startPos.y + endPos.y) / 2.0f };
                    DrawText(TextFormat("%d", weight), midPoint.x + 5, midPoint.y - 15, 20, RED);

                    Vector2 arrowDirPoint = {
                        startPos.x + 0.8f * (endPos.x - startPos.x),
                        startPos.y + 0.8f * (endPos.y - startPos.y)
                    };
                    DrawCircleV(arrowDirPoint, 5.0f, BLUE); 
                }
            }
        }

        for (int i = 0; i < numNodes; i++) {
            DrawCircleV(nodePositions[i], 20.0f, LIGHTGRAY);
            DrawCircleLines(nodePositions[i].x, nodePositions[i].y, 20.0f, BLACK);
            
            int textXOffset = (i < 10) ? 5 : 10; 
            DrawText(TextFormat("%d", i), nodePositions[i].x - textXOffset, nodePositions[i].y - 10, 20, BLACK);
        }

        EndDrawing();
    }

    CloseWindow();
    free(nodePositions);
    
    for (int i = 0; i <= numNodes; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}
