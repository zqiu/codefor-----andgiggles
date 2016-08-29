file = open("input")
blank = False
data = dict()
reverse = dict()
target = ""

for line in file:
    if len(line) != 1:
        key = line[:line.find(" ")]
        val = line[line.find("=> ")+3:-1]
        if blank != True:
            if data.has_key(key):
                data[key].append(val)
            else:
                data[key] = list()
                data[key].append(val)
            if reverse.has_key(val):
                reverse[val].append(key)
            else:
                reverse[val] = list()
                reverse[val].append(key)
        else:
            target = line
    else:
        blank = True

print data

perms = set()
for key in data:
    #print key
    l = data.get(key)
    index = 0
    while index != -1:
        index = target.find(key,index)
        if index == -1:
            break
        else:
            for elem in l:
                perms.add(target[:index] + elem + target[index+len(key):])
            index += 1

print "part 1: %d"%(len(perms))

orderedl = list()
for key in reverse:
    orderedl.append(key)
orderedl.sort(lambda y,x: cmp(len(x), len(y)))
print orderedl

tomodify = target
ans = 0
while tomodify != "e":
    for elem in orderedl:
        index = tomodify.find(elem)
        if index != -1:
            value = reverse[elem][0]
            tomodify = tomodify[:index] + value + tomodify[index+len(elem):]
            ans+=1
            break
print "part 2: %d"%(ans)
