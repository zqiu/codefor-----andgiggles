import string

def isabba(val):
    return val[0] == val[3] and val[1] == val[2] and val[0] != val[1]

def isaba(val):
    return val[0] == val[2] and val[0] != val[1]

def parsesub(val):
    ans = False
    data = string.split(val,"]")
    for a in data:
        l = string.split(a,"[")
        if len(l) == 2 and len(l[1]) > 3:
            for i in range(len(l[1]) - 3):
                if isabba(l[1][i:i+4]):
                    return False
        if len(l[0]) > 3:
            for i in range(len(l[0]) - 3):
                if isabba(l[0][i:i+4]):
                    ans = True
    return ans

def parsesub2(val):
    set1 = set()
    set2 = set()
    data = string.split(val,"]")
    for a in data:
        l = string.split(a,"[")
        if len(l) == 2 and len(l[1]) > 2:
            for i in range(len(l[1]) - 2):
                if isaba(l[1][i:i+3]):
                    set1.add((l[1][i],l[1][i+1]))
        if len(l[0]) > 2:
            for i in range(len(l[0]) - 2):
                if isaba(l[0][i:i+3]):
                    set2.add((l[0][i+1],l[0][i]))
    return len(set1.intersection(set2)) > 0

file = open("input")
ans = 0
ans2 = 0
for line in file:
    if parsesub(line):
        ans += 1
    if parsesub2(line):
        ans2 += 1
print "part1:" + str(ans)
print "part2:" + str(ans2)
