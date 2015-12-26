import re
file = open("input")

def get_sum(string):
	array = re.findall("[-\d]+",string)
	sum = 0
	for value in array:
		sum += int(value)
	return sum

def replace_red(string):
	red = string.find(":\"red\"")
	if red == -1:
		return (0,string)
	count = 1
	start = red
	while count != 0 and start > 0:
		start -= 1
		if string[start] == '{':
			count -= 1
		elif string[start] == '}':
			count += 1
	count = 1
	end = red
	while count != 0 and end < len(string):
		end += 1
		if string[end] == '{':
			count += 1
		elif string[end] == '}':
			count -= 1
	#print string[start:end+1]
	string = string[:start] + string[end+1:]
	return (1,string)
			
for line in file:
	print get_sum(line)
	do_work = 1
	while do_work == 1:
		val = replace_red(line)
		do_work = val[0]
		line = val[1]
	print get_sum(line)