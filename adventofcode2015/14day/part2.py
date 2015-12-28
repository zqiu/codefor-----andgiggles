file = open("input")
data = []
distances = []
scores = []
time = 2503

def do_one_step(timenow):
	for i in range(len(data)):
		if (timenow%(data[i][1] + data[i][2])) < data[i][1]:
			distances[i] += data[i][0]
	winner = distances.index(max(distances))
	scores[winner] += 1

for line in file:
	line = line[:-1].split(" ")
	data.append((int(line[3]),int(line[6]),int(line[13])))
	distances.append(0)
	scores.append(0)
	
for i in range(time):
	do_one_step(i)
	
print max(scores)