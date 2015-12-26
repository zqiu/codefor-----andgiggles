import sys
input = "hepxcrrq"

def valid(string):
	#two different character
	pairs = set()
	for i in range(len(string) - 1):
		if string[i] == string[i+1]:
			pairs.add(string[i])
	if len(pairs) < 2:
		return False
	#incrementing character
	for i in range(len(string) - 2):
		if ord(string[i]) + 1 == ord(string[i + 1]) and ord(string[i + 1]) + 1 == ord(string[i+2]):
			return True
	return False
	
def next(string):
	#find last z
	ans = 0
	for i in range(len(string)):
		char = string[len(string)-i-1]
		if char == 'z':
			ans += 1
		else:
			break
	
	index = -1 *(ans+1)
	increment = chr(ord(string[index])+1)
	if increment == 'i' or increment == 'o' or increment == 'l':
		increment = chr(ord(increment)+1)
	return string[:index]+increment+'a'*ans

for i in range(2):	
	while not valid(input):
		input = next(input)
	print input
	sys.stdout.flush()
	input = next(input)