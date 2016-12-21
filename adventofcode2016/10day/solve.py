file = open("input")
data = {} #map of string names to values
ready = []
opstodo = []
opscopy = []
ans = ""

def addchip(name,value):
    global ans
    if name not in data:
        data[name] = (value,-1)
    elif data[name][1] != -1:
        print "something went wrong"
        print "trying to add %d to %s" %(value,name)
        print "has",data[name]
    else:
        low = data[name][0] if data[name][0] < value else value
        high = data[name][0] if data[name][0] > value else value
        if low == 17 and high == 61:
            ans = name
        data[name] = (low,high)
        ready.append(name)

#tuple with giver,low,high
def doop(op):
    togive = data[op[0]]
    addchip(op[1],togive[0])
    addchip(op[2],togive[1])
        
for line in file:
    line = line[:-1]
    if line[:5] == "value":
        index = line.find(" ")
        index2 = line.find(" ",index+1)
        index3 = line.find("to ") + 3
        addchip(line[index3:],int(line[index:index2]))
    else:
        index = line.find(" gives")
        index2 = line.find("to ") + 3
        index3 = line.find(" and")
        index4 = line.find("to ",index3) + 3
        opstodo.append((line[:index],line[index2:index3],line[index4:]))
        
while len(opstodo):
    opscopy = []
    for op in opstodo:
        if op[0] in ready:
            doop(op)
        else:
            opscopy.append(op)
    opstodo = opscopy

ans2 = data["output 0"][0] * data["output 1"][0] * data["output 2"][0]
print "part1:" + ans
print "part2:" + str(ans2)
