f = open("input")
ans = 0
ans2 = 0
first = ""
for line in f:
	size = len(line)
	for i in range(size - 1):
		if i == 0:
			first = line[0]
		if line[i] == line[i+1]:
			ans += int(line[i])
	for i in range(size):
		if line[i] == line[ (i + size/2) % size]:
			ans2 += int(line[i])
	if first == line[size - 1]:
		ans += int(first)
print ans
print ans2