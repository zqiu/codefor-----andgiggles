f = open("input")
def isdone(possible):
	for val in possible:
		if val[0] != ".":
			return False
	return True
		
possibleroot = []
notroot = []
for line in f:
	splitlist = line.replace(","," ").replace("("," ").replace(")"," ").split()
	if len(splitlist) == 2:
		notroot.append((splitlist[0],int(splitlist[1])))
	else:
		templist = []
		for i in range(3,len(splitlist)):
			templist.append(splitlist[i])
		possibleroot.append([splitlist[0],templist,int(splitlist[1])])

ans = ""
ans2 = 0
changed = 0
while isdone(possibleroot) != True:
	for i in range(len(possibleroot)):
		if possibleroot[i][0] == ".":
			continue
		for j in range(len(possibleroot[i][1])):
			if possibleroot[i][1][j] == ".":
				continue
			if notroot.count(possibleroot[i][1][j]) != 0:
				notroot.remove(possibleroot[i][1][j])
				possibleroot[i][1][j] = "."
		if possibleroot[i][1].count(".") == len(possibleroot[i][1]):
			notroot.append(possibleroot[i][0])
			ans = possibleroot[i][0]
			possibleroot[i][0] = "."
print ans