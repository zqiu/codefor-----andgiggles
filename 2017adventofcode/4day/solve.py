f = open("input")
ans = 0
ans2 = 0
def isanagram(str1,str2):
	if ''.join(sorted(str1)) == ''.join(sorted(str2)):
		return True
	return False
	
def isvalidanagram(inputlist):
	for i in range(len(inputlist) - 1):
		for j in range(i+1,len(inputlist)):
			if isanagram(inputlist[i],inputlist[j]):
				return False
	return True

for line in f:
	words = line.split()
	s = set(words)
	if len(s) == len(words):
		ans += 1
		if isvalidanagram(words):
			ans2 += 1
print ans
print ans2