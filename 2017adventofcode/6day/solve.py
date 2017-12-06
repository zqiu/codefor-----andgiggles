def redistribute(data,num):
	toreturn = []
	sum = data[num]/len(data)
	remainder = data[num] % len(data)
	for i in range(len(data)):
		if i == num:
			toreturn.append(sum)
		else:
			if ((i - num) % len(data)) <= remainder:
				toreturn.append(data[i] + sum + 1)
			else:
				toreturn.append(data[i] + sum)
	return toreturn

def comparelist(l1,l2):
	for a in range(len(l1)):
		if l1[a] != l2[a]:
			return False
	return True
	
ans = 0
ans2 = 0
done = False
f = open("input")
listofthings = []
for line in f:
	data = line.split()
	add = []
	for a in data:
		add.append(int(a))
	listofthings.append(add)
while done == False:
	ans += 1
	modify = listofthings[len(listofthings) - 1]
	val = max(modify)
	for a in range(len(modify)):
		if modify[a] == val:
			listofthings.append(redistribute(modify,a))
			break
	compare = listofthings[len(listofthings) - 1]
	for a in range(len(listofthings) - 2):
		if comparelist(compare,listofthings[a]):
			ans2 = ans - a
			done = True
			break
print ans
print ans2