file = open("input")
rightsue = dict()
question1 = 0

def checkkey(key,value):
	if question1 == 0:
		if key == "cats" or key == "trees":
			return rightsue[key] < value
		if key == "pomeranians" or key == "goldfish":
			return rightsue[key] > value
	return rightsue[key] == value

def init():
	rightsue["children"] = 3
	rightsue["cats"] = 7
	rightsue["samoyeds"] = 2
	rightsue["pomeranians"] = 3
	rightsue["akitas"] = 0
	rightsue["vizslas"] = 0
	rightsue["goldfish"] = 5
	rightsue["trees"] = 3
	rightsue["cars"] = 2
	rightsue["perfumes"] = 1
	
init()
answer = 0
for line in file:
	answer += 1
	line = line.replace(",","")
	line = line.replace(":","")
	line = line[:-1].split(" ")
	if (checkkey(line[2],int(line[3])) and 
		checkkey(line[4],int(line[5])) and
		checkkey(line[6],int(line[7]))):
		break
print answer