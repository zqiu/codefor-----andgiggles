import string

alpha = string.ascii_lowercase

def checksum(val,checksum):
    dic = {}
    for char in val:
        if char == '-':
            continue
        else:
            if char in dic:
                dic[char] += 1
            else:
                dic[char] = 1
    l = sorted(dic.values(),reverse=True)
    keys = sorted(dic.keys())
    ans = ""
    for i in range(5):
        for a in keys:
            if a not in ans and dic[a] == l[i]:
                ans += a
                break
    return ans == checksum

def decryptroom(val,shift):
    global secondans
    ans = ""
    add = shift % len(alpha)
    for char in val:
        if char == '-':
            ans += ' '
        else:
            ans += alpha[(alpha.find(char) + add) % len(alpha)]
    if ans == "northpole object storage":
        secondans = shift

def processline(line):
    val = line[:line.rfind('-')]
    check = line[line.find('[') + 1:line.find(']')]
    if checksum(val,check):
        id = int(line[line.rfind('-') + 1:line.find('[')])
        decryptroom(val,id)
        return id
    return 0

file = open("input")
ans = 0
secondans = 0
for line in file:
    ans +=  processline(line)
print "part1:" + str(ans)
print "part2:" + str(secondans)
