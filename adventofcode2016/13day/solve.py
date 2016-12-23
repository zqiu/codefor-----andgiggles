data = []
inputval = 1350
maxsearch = 100
dest = (31,39)
oddbits = "12478bde"
visited = set()

def getcoord(cor):
    return data[cor[1]][cor[0]]

def iswall(x,y):
    val = (x+3)*x + 2*x*y + (y+1)*y
    val += inputval
    hexval = hex(val)[2:]
    toret = False #starts off even
    for char in hexval:
        if oddbits.find(char) != -1:
            toret = not toret
    return toret
    

for i in range(maxsearch):
    toinsert = ""
    for j in range(maxsearch):
        if iswall(j,i):
            toinsert += "#"
        else:
            toinsert += "."
    data.append(toinsert)

tovisit = set()
tovisit.add((1,1))
ans = 0
ans2 = 0
dir = [(1,0),(-1,0),(0,1),(0,-1)]

while dest not in tovisit:
    next = set()
    for cor in tovisit:
        visited.add(cor)
    for cor in tovisit:
        for additive in dir:
            newcor = (cor[0]+additive[0],cor[1]+additive[1])
            if newcor[0] < 0 or newcor[1] < 0:
                continue
            if newcor not in visited and getcoord(newcor) == ".":
                next.add(newcor)
    ans += 1
    tovisit = next
    if ans == 51: #for 50 days cause we're including your orig location
        ans2 = len(visited)

print "part1:%d"%ans
print "part2:%d"%ans2
