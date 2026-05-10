# OSProject


## Group Members

1. Eden gohari
2. Yonatan ishay
3. israel iday
4. Yonatan Sharify

## File Format

```
<nodes> <edges>
<from> <to> <weight>
...
<source> <destination>
```

Example (`exemple.txt`):
```
6 8
0 1 4
0 2 2
1 3 5
2 1 1
2 3 8
3 4 2
4 5 3
2 5 10
0 5
```

- First line: number of nodes and edges
- Middle lines: each directed edge with its weight
- Last line: source and destination nodes for the shortest path

## Build & Run

### Milestone 1 — read file, run dijkstra, print shortest path and total weight 

```
make milestone1
./dijkstra
```
