file = open("input")
l = []
ans = ""
ans2 = ""
for i in range(8):
    l.append(dict())
for line in file:
    for i in range(8):
        if line[i] in l[i]:
            l[i][line[i]] += 1
        else:
            l[i][line[i]] = 0
for i in range(8):
    vals = sorted(l[i].values(),reverse=True)
    vals2 = sorted(l[i].values())
    keys = l[i].keys()
    for a in keys:
        if l[i][a] == vals[0]:
            ans += a
        if l[i][a] == vals2[0]:
            ans2 += a
print "part1:" + ans
print "part2:" + ans2
