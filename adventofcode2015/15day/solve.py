part1 = 0
file = open("input")
data = []
cal = []
numteaspoons = 100

def find_best_combo(ingredentcount):
	#base case...figured out combination...now just find the score
	if len(ingredentcount) == len(data):
		if part1 == 0:
			calories = 0
			for i in range(len(ingredentcount)):
				calories += cal[i] * ingredentcount[i]
			if calories != 500:
				return 0
		properties = []
		for i in range(len(data[0])):
			properties.append(0)	
		for i in range(len(ingredentcount)):
			for j in range(len(data[0])):
				properties[j] += ingredentcount[i] * data[i][j]
		ans = 1
		for val in properties:
			if val < 0:
				return 0
			else:
				ans *= val
		return ans
	
	#reduction
	currenttotal = sum(ingredentcount)
	outcomes = []
	for i in range(numteaspoons + 1 - currenttotal):
		copy = list(ingredentcount)
		copy.append(i)
		outcomes.append(find_best_combo(copy))
	return max(outcomes)
	
for line in file:
	line = line[:-1].replace(",","").split(" ")
	data.append((int(line[2]),int(line[4]),int(line[6]),int(line[8])))
	cal.append(int(line[10]))
	
print find_best_combo([])