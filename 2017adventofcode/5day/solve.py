f = open("input")
ans = 0
ans2 = 0
instruction = []
copy = []
for line in f:
	instruction.append(int(line))
	copy.append(int(line))
index = 0
while index < len(instruction):
	ans += 1
	val = instruction[index]
	instruction[index] = val+1
	index += val
print ans
#part2
index = 0
instruction = copy
while index < len(instruction):
	ans2 += 1
	val = instruction[index]
	if val >= 3:
		instruction[index] = val-1
	else:
		instruction[index] = val+1
	index += val
print ans2