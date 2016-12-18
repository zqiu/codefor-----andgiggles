import string
import math

file = open("input")
x = 0
y = 0
dir = 0
setdata = set()
ans = (0,0)
found = False

for line in file:
    arr = string.split(line,", ")
    for elem in arr:
        if elem[0] == 'R':
            dir = (dir + 1) % 4
        else:
            dir = (dir - 1) % 4
        mag = int(elem[1:])
        visited = []
        if dir == 0:
            for a in range(mag):
                visited.append((x,y+a))
            y += mag
        elif dir == 1:
            for a in range(mag):
                visited.append((x+a,y))
            x += mag
        elif dir == 2:
            for a in range(mag):
                visited.append((x,y-a))
            y -= mag
        else:
            for a in range(mag):
                visited.append((x-a,y))
            x -= mag
        for tup in visited:
            if tup in setdata and found == False:
                ans = tup
                found = True
            else:
                setdata.add(tup)

print "part1:" + "(" + str(x) + "," + str(y) + ")"
print math.fabs(x) + math.fabs(y)
print "part2:" + str(ans)
print math.fabs(ans[0]) + math.fabs(ans[1])
