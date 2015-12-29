file = open("input")
containers = []
liquid = 150
minnumcontainers = 0
	
def part1():
	return try_containers(0,0,0,1)
def part2():
	return try_containers(0,0,0,0)
	
def try_containers(totalnow,index,containercount,part1):
	global minnumcontainers
	if part1 == 1:
		if totalnow == liquid:
			#found combination
			if containercount < minnumcontainers:
				minnumcontainers = containercount
			return 1
	else:
		if containercount > minnumcontainers:
			return 0
		if totalnow == liquid and containercount == minnumcontainers:
			return 1
	#didn't find combination
	if index == len(containers):
		return 0
	
	#can't add next continer
	if totalnow + containers[index] > liquid:
		return try_containers(totalnow,index+1,containercount,part1)
	else:
	#try when adding container at index and when not adding it
		return (try_containers(totalnow,index+1,containercount,part1) +
		try_containers(totalnow+containers[index],index+1,
		containercount+1,part1)) 
		
for line in file:
	containers.append(int(line[:-1]))	
containers.sort()
minnumcontainers = len(containers)
print "part 1 answer:" + str(part1())
print "min num of containers:" + str(minnumcontainers)
print "part2 answer:" + str(part2())

