#chip,generator
#thulium -> 0&1
#plutonium -> 2&3
#strontium -> 4&5
#promethium -> 6&7
#ruthenium -> 8&9
#elerium -> a&b
#dilithium -> c&d

encoding = "0123456789abcd"
floor1 = "0135"
floor2 = "24"
floor3 = "6789"
floor4 = ""
listofsets = set()

def getval(input):
    return encoding.find(input)

def checkfloor(coor,current,prevfloor):
    l = [prevfloor,current]
    for i in l:
        data = coor[i]
        #no radiation with only 1 or no componenets
        if len(data) > 1:
            haschip = False
            hasgen = False
            for char in data:
                if haschip and hasgen == True:
                    break
                if getval(char)%2 == 0:
                    haschip = True
                else:
                    hasgen = True
            if haschip and hasgen:
                for i in range(len(data)):
                    char = getval(data[i])
                    if char%2 == 0:#is a chip
                        if i == (len(data) - 1) or getval(data[i + 1]) != char+1:
                            return False
    #don't do things that we already went through
    if coor in listofsets:
        return False
    return True

def swapup(op,low,high,index):
    if index != op[4]:
        print "index not matching"
    if index == 0:
        return (low,high,op[2],op[3],index+1)
    elif index == 1:
        return (op[0],low,high,op[3],index+1)
    else:
        return (op[0],op[1],low,high,index+1)

def goupfloor(data,next):
    #add those obtained by going up with one thing
    floor = data[4]
    for i in range(len(data[floor])):
        low = data[floor][:i] + data[floor][i+1:]
        high = "".join(sorted(data[floor][i] + data[floor + 1]))
        toadd = swapup(data,low,high,floor)
        if checkfloor(toadd,floor,floor+1):
            next.add(toadd)
            
    #add those obtained by going up wth two things
    for i in range(len(data[floor])):
        for j in range(i+1,len(data[floor])):
            high = "".join(sorted(data[floor][i] + data[floor][j] + data[floor+1]))
            low = data[floor][:i] + data[floor][i+1:j] + data[floor][j+1:]
            toadd = swapup(data,low,high,floor)
            if checkfloor(toadd,floor,floor+1):
                next.add(toadd)

def swapdown(op,low,high,index):
    if index != op[4]:
        print "index not matching"
    if index == 1:
        return (low,high,op[2],op[3],index-1)
    elif index == 2:
        return (op[0],low,high,op[3],index-1)
    else:
        return (op[0],op[1],low,high,index-1)

def godownfloor(data,next):
    #add those obtained by going down with one thing
    floor = data[4]
    for i in range(len(data[floor])):
        high = data[floor][:i] + data[floor][i+1:]
        low = "".join(sorted(data[floor][i] + data[floor - 1]))
        toadd = swapdown(data,low,high,floor)
        if checkfloor(toadd,floor,floor-1):
            next.add(toadd)
            
    #add those obtained by going down wth two things
    for i in range(len(data[floor])):
        for j in range(i+1,len(data[floor])):
            low = "".join(sorted(data[floor][i] + data[floor][j] + data[floor-1]))
            high = data[floor][:i] + data[floor][i+1:j] + data[floor][j+1:]
            toadd = swapdown(data,low,high,floor)
            if checkfloor(toadd,floor,floor-1):
                next.add(toadd)

                
possible = set()
possible.add((floor1,floor2,floor3,floor4,0))
solved = False
ans = 0

while not solved:
    next = set()
    for val in possible:
        if val[4] != 3:
            goupfloor(val,next)
        if val[4] != 0:
            trylower = True
            for i in range(val[4]):
                if val[i] != "":
                    break
                if i == (val[4] - 1):
                    trylower = False
            if trylower:
                godownfloor(val,next)
    ans += 1
    if ans%10 == 0:
        print "tried %d"%ans
    listofsets = listofsets.union(possible)
    possible = next
    for val in possible:
        if val[0] == "" and val[1] == "" and val[2] == "":
            solved = True

print "ans:" + str(ans)
