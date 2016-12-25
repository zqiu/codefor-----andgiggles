INPUT = ".^^..^...^..^^.^^^.^^^.^^^^^^.^.^^^^.^^.^^^^^^.^...^......^...^^^..^^^.....^^^^^^^^^....^^...^^^^..^"
traps = ["^^.",".^^","^..","..^"]

def gettile(left,center,right):
    for a in traps:
        if left == a[0] and center == a[1] and right == a[2]:
            return "^"
    return "."


ans = 0
ans2 = 0
ans += INPUT.count(".")
ans2 += INPUT.count(".")

for i in range(399999):
    tmp = ""
    for j in range(len(INPUT)):
        left = "." if j == 0 else INPUT[j-1]
        right = "." if j == (len(INPUT) - 1) else INPUT[j+1]
        tmp += gettile(left,INPUT[j],right)
    INPUT = tmp
    if i < 40:
        ans += INPUT.count(".")
    ans2 += INPUT.count(".")

print "part 1:%d"%ans
print "part 2:%d"%ans2
