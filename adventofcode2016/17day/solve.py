import hashlib

INPUT = "qzthpkfp"
VALID = "bcdef"
MAPPING = [("U",(-1,0)),("D",(1,0)),("L",(0,-1)),("R",(0,1))]
m = hashlib.md5(INPUT)

totry = [((0,0),"")]
ans = ""
ans2 = 0
correctpaths = []

def isvalid(char):
    for a in VALID:
        if char == a:
            return True
    return False

def iscordvalid(cor):
    if cor[0] == 3 and cor[1] == 3:
        return True
    return False

def dodir(dirct,cord,l,prev):
    global correctpaths
    for op in MAPPING:
        if dirct == op[0]:
            newcord = (cord[0]+op[1][0],cord[1]+op[1][1])
            valid = iscordvalid(newcord)
            path = prev + dirct
            if not valid:
                l.append((newcord,path))
            else:
                correctpaths.append(path)
            return valid,prev+dirct

while len(totry) != 0:
    temp = []
    for elem in totry:
        copy = m.copy()
        copy.update(elem[1])
        digest = copy.hexdigest()
        address = elem[0]
        if isvalid(digest[0]) and address[0] != 0:
            a,b = dodir("U",address,temp,elem[1])
            if a and ans == "":
                ans = b
        if isvalid(digest[1]) and address[0] != 3:
            a,b = dodir("D",address,temp,elem[1])
            if a and ans == "":
                ans = b
        if isvalid(digest[2]) and address[1] != 0:
            a,b = dodir("L",address,temp,elem[1])
            if a and ans == "":
                ans = b
        if isvalid(digest[3]) and address[1] != 3:
            a,b = dodir("R",address,temp,elem[1])
            if a and ans == "":
                ans = b
    totry = temp

print "ans1:" + ans
for itr in correctpaths:
    if len(itr) > ans2:
        ans2 = len(itr)
print "ans2:%d"%ans2
