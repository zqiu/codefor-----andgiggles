file = open("input")

ans = 0
ans2 = 0
for line in file:
	str = line[:-1]
	ans += len(str) - len(str[1:-1].decode('string_escape'))
	str2 = line[1:-2]
	ans2 += 4 #to encode \"\"
	ans2 += str2.count('\\')
	ans2 += str2.count('"')
	
print ans
print ans2