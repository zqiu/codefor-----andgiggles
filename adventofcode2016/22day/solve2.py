masterlist = []

datafile = open("input")

templist = []
for line in datafile:
    split = line.split()
    used = int(split[2][:-1])
    size = int(split[1][:-1])
    templist.append((used,size))
    if len(templist) == 27:
        masterlist.append(templist)
        templist = []

grid = []
empty = (0,0)
for i in range(len(masterlist)):
    data = ""
    for j in range(len(masterlist[i])):
        if masterlist[i][j][0] == 0:
            data += '+'
            empty = (i,j)
        elif masterlist[i][j][1] > 100:
            #large node
            data += '#'
        elif i == (len(masterlist) - 1) and j == 0:
            data += 'S'
        elif i == 0 and j == 0:
            data += 'G'
        else:
            data += '.'
    grid.append(data)

for row in grid:
    print row
    
ans = 0
hitwall = False
while grid[empty[0]][empty[1]] != 'S':
    if hitwall:
        if empty[1] == 0:
            empty = (empty[0] + 1, empty[1])
        elif grid[empty[0]][empty[1] - 1] != '#':
            empty = (empty[0], empty[1] - 1)
        else:
            empty = (empty[0] - 1, empty[1])
    else:
        empty = (empty[0], empty[1] - 1)
        if grid[empty[0]][empty[1] - 1] == '#':
            hitwall = True
    ans += 1

print "part 2:",(ans + 5 * (len(masterlist) - 2))
