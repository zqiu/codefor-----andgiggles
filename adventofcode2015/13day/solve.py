data = {}
names = set()
file = open("input")

def add_dic(name1,name2,value):
	names.add(name1)
	names.add(name2)
	val1 = name1 if name1 < name2 else name2
	val2 = name2 if name1 < name2 else name1
	if (val1,val2) in data:
		data[(val1,val2)] += value
	else:
		data[(val1,val2)] = value
	data[("me",val1)] = 0
	data[("me",val2)] = 0

def find_val(name1,name2):
	val1 = name1 if name1 < name2 else name2
	val2 = name2 if name1 < name2 else name1
	if val2 == "me":
		temp = val1
		val1 = val2
		val2 = temp
	return data[(val1,val2)]

def find_happiness(start,now,remaining):
	values = []
	if len(remaining) == 0:
		return find_val(start,now)
	else:
		for i in range(len(remaining)):
			new_remaining = remaining[:i]+remaining[i+1:]
			val = find_val(now,remaining[i]) + find_happiness(start,remaining[i],new_remaining)
			values.append(val)
		return max(values)

for line in file:
	line = line[:-2].split(" ")
	add_dic(line[0],line[10],-1*int(line[3]) if line[2] == "lose" else int(line[3]))
#print data
names = list(names)
#print names
print find_happiness(names[0],names[0],names[1:])
names.append("me")
print find_happiness(names[0],names[0],names[1:])