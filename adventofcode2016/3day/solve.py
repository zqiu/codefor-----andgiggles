import string

def ispossible(x,y,z):
    if (x+y)>z and (x+z)>y and (y+z)>x: 
        return True
    return False

tot = []
file = open("input")
valid = 0
valid2 = 0
for line in file:
    arr = string.split(line.lstrip(),' ')
    l = []
    for itr in arr:
        if itr != "":
            l.append(itr)
    if ispossible(int(l[0]),int(l[1]),int(l[2])):
        valid += 1
    tot.append(l)
    if len(tot) == 3:
        for i in range(3):
            if ispossible(int(tot[0][i]),int(tot[1][i]),int(tot[2][i])):
                valid2 += 1
        tot = []
        
print "part 1:" + str(valid)
print "part 2:" + str(valid2)
