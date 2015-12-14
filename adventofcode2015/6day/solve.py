question = 1
data = []
ans = 0
file = open("input")

def do_lights(xmin,ymin,xmax,ymax,type):
	#print str(xmin) + " " + str(xmax) + " " + str(ymin) + " " + str(ymax)
	for x in range(xmin,xmax+1):
		for y in range(ymin,ymax+1):
			if question == 0:
				if type < 2:		#on/off
					data[x][y] = type
				elif type == 2:	#toggle
					data[x][y] = 1 if data[x][y] == 0 else 0
			else:
				data[x][y] = max(0,data[x][y] + type)
					
for i in range(1000):
	thing = []
	for j in range(1000):
		thing.append(0)
	data.append(thing)
		
for line in file:
	info = line[:-1].replace(" ",",").split(",")
	if len(info) == 7:			#turn on/off
		if question == 0:
			do_lights(int(info[2]),int(info[3]),int(info[5]),int(info[6]),
				0 if info[1] == "off" else 1)
		else:
			do_lights(int(info[2]),int(info[3]),int(info[5]),int(info[6]),
				-1 if info[1] == "off" else 1)
	elif len(info) == 6:		#toggle
		if question == 0:
			do_lights(int(info[1]),int(info[2]),int(info[4]),int(info[5]),2)
		else:
			do_lights(int(info[1]),int(info[2]),int(info[4]),int(info[5]),2)			
			
for i in range(1000):
	for j in range(1000):
		if question == 0:
			if data[i][j] == 1:
				ans += 1
		else:
			ans += data[i][j]
print ans