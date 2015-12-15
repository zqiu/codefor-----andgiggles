input = "1113222113"

def do_one_transform(string):
	ans = ""
	thing = string[0]
	num = 0
	for char in string:
		if char != thing:
			ans = ans + str(num) + thing
			num = 1
			thing = char
		else:
			num += 1
	ans = ans + str(num) + thing
	return ans
		
for i in range(40):
	input = do_one_transform(input)

print len(input)