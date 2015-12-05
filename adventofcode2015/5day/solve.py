file = open("input")

def numvowels(line):
	numvowels = 0
	for char in line:
		if char == 'a' or char == 'e' or char == 'i' or char == 'o' or char == 'u':
			numvowels += 1
	return numvowels

def repeatingletter(line):
	if len(line) < 3:
		return False
	for i in range(len(line) - 2):
		if line[i] == line[i + 2]:
			return True
	return False
	
def pairpair(line):
	pairs = set()
	i = 0
	while i < len(line) - 1:
		#triplets
		if i < len(line) - 2 and line[i] == line[i + 1] and line[i + 1] == line[i + 2]:
			if line[i:i+2] in pairs:
				return True
			else:
				pairs.add(line[i:i+2])
			i += 1
		else:
			if line[i:i+2] in pairs:
				return True
			else:
				pairs.add(line[i:i+2])
		i += 1
	return False
	
ans1 = 0
ans2 = 0
for thing in file:
	if numvowels(thing) >= 3:
		repeat = 0
		for i in range(len(thing) - 1):
			if repeat == 0 and thing[i] == thing[i+1]:
				repeat = 1
			if thing[i] == 'a' and thing[i+1] == 'b':
				repeat = 0
				break
			if thing[i] == 'c' and thing[i+1] == 'd':
				repeat = 0
				break
			if thing[i] == 'p' and thing[i+1] == 'q':
				repeat = 0
				break
			if thing[i] == 'x' and thing[i+1] == 'y':
				repeat = 0
				break
		if repeat == 1:
			ans1 += 1
	if repeatingletter(thing) and pairpair(thing):
		ans2 += 1
			
print ans1
print ans2