f = open("input")
ans = 0
ans2 = 0
for line in f:
	size = len(line)
	for i in range(size):
		if line[i] == line[(i+1)%size]:
			ans += int(line[i])
		if line[i] == line[ (i + size/2) % size]:
			ans2 += int(line[i])
print ans
print ans2