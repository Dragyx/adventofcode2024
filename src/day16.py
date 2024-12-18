from heapq import heapify, heappop, heappush

INF = 100000000000000

grid = []
goal = (0, 0)
deer = (0, 0)

with open("inputs/input16.txt", "r") as f:
    for i, line in enumerate(f.readlines()):
        grid.append([c if c == '#' else INF  for c in line.strip() ])
        for j, c in enumerate(line):
            if (c == 'S'):
                deer = (i, j)
            elif (c == 'E'):
                goal = (i, j)


def pgrid():
    for i, row in enumerate(grid):
        for j, field in enumerate(row):
            if (i, j) == goal:
                print("G", end="")
            elif (i, j) == deer:
                print("D", end="")
            else:
                print("#" if field == "#" else " ", end="")
        print()

def get_neighbors(grid, i, j):
    w = len(grid[0])
    h = len(grid)
    return filter(
                lambda p: p[0] >= 0 and p[0] < h and p[1] >= 0 and p[1] <= w, 
                [(i + 1, j), (i - 1, j), (i, j - 1), (i, j + 1)]
            )

def find_min(grid, deer, goal):
    Q = []

    grid[deer[0]][deer[1]] = 0
    for i, row in enumerate(grid):
        for j, c in enumerate(row):
            if (c is not None and c != '#'):
                heappush(Q, (c, (i, j)))
            
    while len(Q) != 0:
        (i, j) = heappop(Q)
        for (k, l) in get_neighbors(grid, i, j):
            

    
                
pgrid()
print("Start: ", deer)
print("Goal:  ", goal)
                
        
