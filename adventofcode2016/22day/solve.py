masterlist = []

def obtain(val):
    length = len(masterlist[0])
    x = val / length
    y = val % length
    return masterlist[x][y]

datafile = open("input")
templist = []
for line in datafile:
    split = line.split()
    used = int(split[2][:-1])
    avail = int(split[3][:-1])
    templist.append((used,avail))
    if len(templist) == 27:
        masterlist.append(templist)
        templist = []

part1 = 0
size = len(masterlist) * len(masterlist[0])
for i in range(size):
    for j in range(size):
        if i == j:
            continue
        A = obtain(i)
        B = obtain(j)
        if A[0] != 0 and A[0] <= B[1]:
            part1 += 1

print "part1:%s"%part1
