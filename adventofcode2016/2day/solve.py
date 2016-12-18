file = open("input")
h = "123456789ABCD"
v1 = "26A"
v2 = "137BD"
v3 = "48C"

def cutoff(a,b,c):
    if a < b:
        return b
    if a > c:
        return c
    return a

def hmov(val,dir):
    rem = (val-1)%3
    rem += dir
    rem = cutoff(rem,0,2)
    return ((val-1)/3)*3+rem+1

def vmov(val,dir):
    rem = (val-1)/3
    rem += dir
    rem = cutoff(rem,0,2)
    return ((val-1)%3)+rem*3+1

def hmov2(val,dir):
    index = h.find(val)
    if index == 0 or index == (len(h) - 1):
        return h[index]
    elif index <= 3:
        return h[cutoff(index+dir,1,3)]
    elif index <= 8:
        return h[cutoff(index+dir,4,8)]
    else:
        return h[cutoff(index+dir,9,11)]

def vmov2(val,dir):
    l = []
    if v1.find(val) != -1:
        l = v1
    elif v2.find(val) != -1:
        l = v2
    elif v3.find(val) != -1:
        l = v3
    else:
        return val
    index = l.find(val)
    index += dir
    index = cutoff(index,0,len(l)-1)
    return l[index]
    
start = 5
start2 = "5"
ans = ""
ans2 = ""
for line in file:
    for char in line:
        if char == 'U':
            start = vmov(start,-1)
            start2 = vmov2(start2,-1)
        elif char == 'D':
            start = vmov(start,1)
            start2 = vmov2(start2,1)
        elif char == 'R':
            start = hmov(start,1)
            start2 = hmov2(start2,1)
        elif char == 'L':
            start = hmov(start,-1)
            start2 = hmov2(start2,-1)
    ans += str(start)
    ans2 += start2
print "part 1:" + ans
print "part 2:" + ans2
