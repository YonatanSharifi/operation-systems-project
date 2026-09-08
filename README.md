# OS Project - Graph Simulation

## Team Members
* Eden Gohari
* Yonatan Ishay
* Israel Iday
* Yonatan Sharify

## Overview
A simulation of an entity moving on a directed graph, written in C.
The shortest path is calculated using Dijkstra's algorithm, and the visualization is built with the Raylib library.

## Project Stages
* **Milestone 1:** Dijkstra's algorithm implementation (console output).
* **Milestone 2:** Static graph rendering (nodes arranged in a circle, directed edges with weights).
* **Milestone 3:** Entity movement animation along the calculated path.
* **Milestone 4:** Multiple travelers moving at once, each one simulated by its own child process.

## Files
* `dijkstra.c` / `dijkstra.h` — reading the basic graph file and the Dijkstra shortest-path algorithm.
* `graphics.c` / `graphics.h` — shared Raylib drawing code (node layout, edges/arrows, the graph itself).
* `travelers.c` / `travelers.h` — reading milestone 4's extended input file (the `# travelers` section).
* `milestone1.c` … `milestone4.c` — the entry point of each milestone, built on top of the files above.

## Build
Use the provided Makefile to compile:
* `make milestone1`
* `make milestone2`
* `make milestone3`
* `make milestone4`
* `make clean` — removes all compiled binaries (`dijkstra`, `sim`, object files)

## Run
Milestone 1:
`./dijkstra <input_file>`

Milestones 2, 3 & 4:
`./sim <input_file>`

### Milestone 1 — read file, run dijkstra, print shortest path and total weight 

### Milestone 2 — draw the graph (nodes in a circle, directed edges with their weights) with raylib

### Milestone 3 — animate a single entity moving along the shortest path computed by Dijkstra
### Milestone 4 — multiple travelers, one child process per traveler

The input file gets a `# travelers` section (count + `source dest` pairs per
line, see `graph_travelers.txt`). The parent computes each traveler's
Dijkstra path, then `fork()`s one child per traveler — each child just
prints `[PID] started` and sleeps (`pause()`); all the real work (the GUI
loop, moving every traveler) is done by the parent. All travelers move
together in the same window, each in a different color, controlled by one
shared PLAY/STOP button. When a traveler finishes, the parent signals and
waits for its child; it makes sure every child is signaled and waited for
before exiting.


### Milestone 1 — read file, run dijkstra, print shortest path and total weight 

```
make milestone1
./dijkstra
```
