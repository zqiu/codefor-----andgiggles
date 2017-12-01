start = (0,0)
points = []
maze = []

mazefile = open("maze")
row = 0
for line in mazefile:
	if line[1] != '#':
		maze.append(line[1:-1])
#for 
