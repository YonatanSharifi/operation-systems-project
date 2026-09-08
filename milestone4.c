#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "raylib.h"
#include "dijkstra.h"
#include "graphics.h"
#include "travelers.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SECONDS_PER_WEIGHT_UNIT 0.3f
#define CHILD_END_SIGNAL SIGTERM
#define STATE_MOVING 0
#define STATE_FINISHED 1

typedef struct { //each traveler data
    int *path;
    int path_len;
    int state;
    int current_edge;
    float time_on_edge;
    Vector2 pos;
    Color color;
    pid_t pid;
    int child_ended;
} Traveler;

static void run_child(void) { //print each son started
    printf("[%d] started\n", getpid());
    fflush(stdout);
    while (1) pause();
}

static void end_traveler_child(Traveler *t) { //end child run
    if (t->child_ended) return;
    kill(t->pid, CHILD_END_SIGNAL);
    waitpid(t->pid, NULL, 0);
    t->child_ended = 1;
}

int main(int argc, char *argv[]) {
    if (argc < 2) { //check if we got file
        printf("Missing file\n");
        return 1;
    }
    // initialize data
    TravelerSpec *specs = NULL;
    int num_travelers = 0;
    int **matrix = read_multi_traveler_file(argv[1], &specs, &num_travelers);
    if (!matrix) {
        perror("Open file failed");
        return 1;
    }
    int num_nodes = matrix[0][0];

    if (num_travelers <= 0) { //check if there is travelers
        printf("No travelers found in input file\n");
        for (int i = 0; i <= num_nodes; i++) free(matrix[i]);
        free(matrix);
        free(specs);
        return 1;
    }

    Traveler *travelers = malloc(num_travelers * sizeof(Traveler)); //initialize travelers
    for (int i = 0; i < num_travelers; i++) { //check each traveler path and his data
        travelers[i].path = compute_path(matrix, specs[i].source, specs[i].dest, &travelers[i].path_len);
        travelers[i].state = STATE_MOVING;
        travelers[i].current_edge = 0;
        travelers[i].time_on_edge = 0.0f;
        travelers[i].child_ended = 0;
        travelers[i].color = ColorFromHSV((float)(i * (360.0 / num_travelers)), 0.85f, 0.95f);
    }

    for (int i = 0; i < num_travelers; i++) { //create son for each traveler
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(1);
        }
        if (pid == 0) {
            run_child();
            exit(0);
        }
        travelers[i].pid = pid;
    }
    //initialize graphics
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OS Project - Milestone 4");
    SetTargetFPS(60);
    //nodes position
    Vector2 *positions = malloc(num_nodes * sizeof(Vector2));
    Vector2 center;
    center.x = SCREEN_WIDTH / 2.0f;
    center.y = SCREEN_HEIGHT / 2.0f;
    float radius = 220.0f;
    compute_circle_positions(num_nodes, center, radius, positions);
    //each traveler starting node, and check if each traveler start node = end node
    for (int i = 0; i < num_travelers; i++) {
        Traveler *t = &travelers[i];
        if (t->path_len <= 1) {
            t->state = STATE_FINISHED;
            if (t->path_len == 1) {
                t->pos = positions[t->path[0]];
            } else {
                t->pos = center;
            }
            end_traveler_child(t);
        } else {
            t->pos = positions[t->path[0]];
        }
    }
    //botton
    bool is_playing = false;
    Rectangle btnBounds;
    btnBounds.x = 10;
    btnBounds.y = 10;
    btnBounds.width = 100;
    btnBounds.height = 40;

    while (!WindowShouldClose()) {
        Vector2 mousePoint = GetMousePosition();
        //check if botton pressed
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePoint, btnBounds)) {
            is_playing = !is_playing;
        }
        //if not playing make sure nothing moves
        float dt;
        if (is_playing) {
            dt = GetFrameTime();
        } else {
            dt = 0.0f;
        }

        for (int i = 0; i < num_travelers; i++) {
            Traveler *t = &travelers[i];
            if (t->state != STATE_MOVING) continue;

            t->time_on_edge += dt;
            int u = t->path[t->current_edge];
            int v = t->path[t->current_edge + 1];
            int weight = matrix[u + 1][v];
            if (weight <= 0) weight = 1;
            float edge_duration = weight * SECONDS_PER_WEIGHT_UNIT;

            if (t->time_on_edge >= edge_duration) {
                t->current_edge++;
                t->time_on_edge = 0.0f;
                if (t->current_edge >= t->path_len - 1) {
                    t->state = STATE_FINISHED;
                    t->pos = positions[t->path[t->path_len - 1]];
                    end_traveler_child(t);
                } else {
                    t->pos = positions[t->path[t->current_edge]];
                }
            } else {
                float fraction = t->time_on_edge / edge_duration;
                t->pos.x = positions[u].x + (positions[v].x - positions[u].x) * fraction;
                t->pos.y = positions[u].y + (positions[v].y - positions[u].y) * fraction;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        draw_graph(matrix, num_nodes, positions);

        int finished_count = 0;
        for (int i = 0; i < num_travelers; i++) {
            Traveler *t = &travelers[i];
            DrawCircleV(t->pos, 10.0f, t->color);
            if (t->state == STATE_FINISHED) finished_count++;
        }

        DrawRectangleRec(btnBounds, is_playing ? RED : GREEN);
        DrawText(is_playing ? "STOP" : "PLAY", 35, 20, 20, WHITE);

        char status[64];
        sprintf(status, "Travelers finished: %d / %d", finished_count, num_travelers);
        DrawText(status, 10, 60, 20, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();

    for (int i = 0; i < num_travelers; i++) {
        end_traveler_child(&travelers[i]);
        if (travelers[i].path) free(travelers[i].path);
    }

    free(positions);
    free(travelers);
    free(specs);
    for (int i = 0; i <= num_nodes; i++) free(matrix[i]);
    free(matrix);
    return 0;
}
