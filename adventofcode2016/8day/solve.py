grid = []
mini = []
for i in range(50):
    mini.append(".")
for i in range(6):
    grid.append(mini[:])

def rect(x,y):
    global grid
    for i in range(x):
        for j in range(y):
            grid[j][i] = "#"

def rc(x,mag):
    global grid
    rot = mag%6
    new = []
    for i in range(6):
        new.append(grid[(i-rot)%6][x])
    for i in range(6):
        grid[i][x] = new[i]

def rr(y,mag):
    global grid
    rot = mag%50
    new = []
    for i in range(50):
        new.append(grid[y][(i-rot)%50])
    for i in range(50):
        grid[y][i] = new[i]

file = open("input")
for line in file:
    chunked = line.split(" ")
    if chunked[0] == "rect":
        vals = chunked[1].split("x")
        rect(int(vals[0]),int(vals[1]))
    if chunked[0] == "rotate":
        vals = chunked[2].split("=")
        if vals[0] == "x":
            rc(int(vals[1]),int(chunked[4]))
        else:
            rr(int(vals[1]),int(chunked[4]))

ans = 0
for i in range(6):
    for j in range(50):
       if grid[i][j] == "#":
           ans += 1

print "part1:" + str(ans)
print "part2:"
for i in range(6):
    grid[i] = "".join(grid[i])
    print grid[i]
