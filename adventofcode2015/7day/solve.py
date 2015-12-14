data = {}
undetermined = []
temp = []
file = open("input")

def in_dictionary(val):
	#print val
	return val in data or val.isdigit()

def get_dic_val(val):
	if val.isdigit():
		return int(val)
	else:
		return data[val]

def try_operation(a,b,c,operation,toadd):
	if in_dictionary(a) and in_dictionary(b):
		if operation == "=":
			data[c] = get_dic_val(a)
		elif operation == "NOT":
			data[c] = ~get_dic_val(a)
		elif operation == "AND":
			data[c] = get_dic_val(a) & get_dic_val(b)
		elif operation == "OR":
			data[c] = get_dic_val(a) | get_dic_val(b)
		elif operation == "RSHIFT":
			data[c] = get_dic_val(a) >> get_dic_val(b)
		elif operation == "LSHIFT":
			data[c] = get_dic_val(a) << get_dic_val(b)
	else:
		if toadd == 0:
			undetermined.append([a,b,c,operation])
		else:
			temp.append([a,b,c,operation])			
			
for line in file:
	slice = line[:-1].split(" ")
	if(len(slice) == 3):
		try_operation(slice[0],"0",slice[2], "=",0)				#store
	elif(len(slice) == 4):
		try_operation(slice[1],"0",slice[3], slice[0],0)		#not
	else:
		try_operation(slice[0],slice[2],slice[4], slice[1],0)	#others

while len(undetermined):
	temp = []
	for thing in undetermined:
		try_operation(thing[0],thing[1],thing[2],thing[3],1)
	undetermined = temp

print data["a"]	