file = open("input")
data = []
numlights = 100
part1 = 0

def query_next_state(x,y):
	numneighborson = 0
	if part1 == 0:
		if (x == 0 or x == numlights - 1) and (y == 0 or y == numlights -1):
			return '#'
	for i in range(-1,2):
		if x+i < 0 or x+i >= numlights:
			continue
		for j in range(-1,2):
			if y+j < 0 or y+j >= numlights:
				continue
			if i == 0 and j == 0:
				continue
			if data[x+i][y+j] == '#':
				numneighborson += 1
	if data[x][y] == '#' and (numneighborson < 2 or numneighborson > 3):
		return '.'
	if data[x][y] == '.' and (numneighborson == 3):
		return '#'
	return data[x][y]

def next_step():
	global data
	newdata = []
	for i in range(numlights):
		line = ''
		for j in range(numlights):
			line += query_next_state(i,j)
		newdata.append(line)
	data = newdata
		
for line in file:
	data.append(line[:-1])
for i in range(100):
	next_step()
ans = 0
for i in range(numlights):
	ans += data[i].count('#')
print ans
	