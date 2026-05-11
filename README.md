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
* **Milestone 3:** Entity movement animation along the calculated path (300ms delay per edge weight unit, 1-second delay at intermediate nodes).

## Build
Use the provided Makefile to compile:
* `make milestone1`
* `make milestone2`
* `make milestone3`
* `make clean`

## Run
Milestone 1:
`./dijkstra <input_file>`

Milestones 2 & 3:
`./sim <input_file>`

### Milestone 1 — read file, run dijkstra, print shortest path and total weight 

```
make milestone1
./dijkstra
```
