file = open("input")
data = []
cities = []
ans = 0
question = 0

def city_distance(start,end):
	for slice in data:
		if slice[0] == start and slice[1] == end:
			return int(slice[2])
		elif slice[1] == start and slice[0] == end:
			return int(slice[2])
	print "coulden't find " + start + " and " + end

def get_smallest_dist(curcity,remainingcities):
	distance = -1
	if len(remainingcities) == 0:
		return 0
	for cities in remainingcities:
		copy = list(remainingcities)
		copy.remove(cities)
		val = city_distance(curcity,cities) + get_smallest_dist(cities,copy)
		if distance == -1:
			distance = val
		else:
			if question == 0:
				distance = min(val,distance)
			else:
				distance = max(val,distance)
	return distance	

for line in file:
	info = line[:-1].replace("to"," ").replace("="," ").split()
	data.append(info)
	if info[0] not in cities:
		cities.append(info[0])
	if info[1] not in cities:
		cities.append(info[1])
		
for city in cities:
	copy = list(cities)
	copy.remove(city)
	val = get_smallest_dist(city,copy)
	if ans == 0:
		ans = val
	else:
		if question == 0:
			ans = min(val,ans)
		else:
			ans = max(val,ans)
print ans
	