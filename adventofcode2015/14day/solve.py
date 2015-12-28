file = open("input")
data = []
time = 2503

def get_distance(raindeer):
	flyingtime = (time/(raindeer[1] + raindeer[2]))*raindeer[1]
	flyingtime += min(raindeer[1],time%(raindeer[1] + raindeer[2]))
	return flyingtime * raindeer[0]
	
for line in file:
	line = line[:-1].split(" ")
	data.append((int(line[3]),int(line[6]),int(line[13])))

distances = []
for raindeer in data:
	distances.append(get_distance(raindeer))

print max(distances)