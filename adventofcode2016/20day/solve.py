File = open("input")

ips = []
for line in File:
    data = line[:-1].split("-")
    ips.append((int(data[0]),int(data[1])))
ips = sorted(ips)

def getupper(low, orig):
    toret = orig
    waschanged = True
    while waschanged == True:
        waschanged = False
        for a in ips:
            if a[0] <= (toret + 1) and a[0] >= low and a[1] > toret:
                toret = a[1]
                waschanged = True
    return toret

index = 0
while index < len(ips):
    v = ips[index]
    data = getupper(v[0],v[1])
    tmp = ips[:index]
    tmp.append((v[0],data))
    i = 0
    while i < len(ips) and data > ips[i][0]:
        i += 1
    tmp += ips[i:]
    ips = tmp
    index += 1
    
print "ans 1:%d"%(ips[0][1]+1)
ans2 = 0
for i in range(len(ips) - 1):
    ans2 += ips[i+1][0] - (ips[i][1] + 1)
print "ans 2:%d"%ans2
