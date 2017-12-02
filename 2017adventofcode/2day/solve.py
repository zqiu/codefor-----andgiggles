f = open("input")
ans = 0
ans2 = 0
for line in f:
	splited = line.split()
	min = int(splited[0])
	max = int(splited[0])
	for a in splited:
		if int(a) < min:
			min = int(a)
		if int(a) > max:
			max = int(a)
	ans += (max - min)
	for a in range(len(splited) - 1):
		for b in range(a + 1,len(splited)):
			if int(splited[a]) % int(splited[b]) == 0:
				ans2 += int(splited[a]) / int(splited[b])
				added = True
			elif int(splited[b]) % int(splited[a]) == 0:
				ans2 += int(splited[b]) / int(splited[a])
print ans
print ans2