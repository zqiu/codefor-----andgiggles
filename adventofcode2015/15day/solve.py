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
		ans = 1
		for i in range(len(data[0])):
			currentproperty = 0
			for j in range(len(ingredentcount)):
				currentproperty += ingredentcount[j] * data[j][i]
			if currentproperty < 0:
				return 0
			else:
				ans *= currentproperty
		return ans
	
	#reduction
	currenttotal = sum(ingredentcount)
	score = 0
	for i in range(numteaspoons + 1 - currenttotal):
		ingredentcount.append(i)
		possible_score = find_best_combo(ingredentcount)
		ingredentcount.pop()
		if score < possible_score:
			score = possible_score
	return score
		
for line in file:
	line = line[:-1].replace(",","").split(" ")
	data.append((int(line[2]),int(line[4]),int(line[6]),int(line[8])))
	cal.append(int(line[10]))
	
print find_best_combo([])