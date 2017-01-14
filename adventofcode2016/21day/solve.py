#x,y are ints
def swap_pos(string,x,y):
    in1 = min(x,y)
    in2 = max(x,y)
    return string[:in1] + string[in2] + string[in1+1:in2] + string[in1] + string[in2+1:]

#x,y are chars
def swap_letter(string,x,y):
    return swap_pos(string,string.find(x),string.find(y))

#direct = str, mag = int
def rot(string,direct,mag):
    length = len(string)
    mag = mag%length
    if direct == "right":
        return string[length - mag:] + string[:length - mag]
    elif direct == "left":
        return string[mag:] + string[:mag]
    else:
        print "invalid direction"
        return

#x is a char
def rotpos(string,x):
    pos = string.find(x)
    if pos >= 4:
        pos += 1
    return rot(string,"right",pos + 1)

#x,y are ints
def reversepos(string,x,y):
    in1 = min(x,y)
    in2 = max(x,y)
    temp = string[in1:in2+1]
    return string[:in1] + temp[::-1] + string[in2+1:]

#x,y are ints
def movepos(string,x,y):
    index = string[x]
    temp = string[:x] + string[x+1:]
    return temp[:y] + index + temp[y:]

ans = "abcdefgh"
data = open("input")
total = []

for line in data:
    split = line[:-1].split(" ")
    total.append(split)
    if split[0] == "swap":
        if split[1] == "position":
            ans = swap_pos(ans,int(split[2]),int(split[5]))
        else:
            ans = swap_letter(ans,split[2],split[5])
    elif split[0] == "rotate":
        if split[1] == "based":
            ans = rotpos(ans,split[6])
        else:
            ans = rot(ans,split[1],int(split[2]))
    elif split[0] == "reverse":
        ans = reversepos(ans,int(split[2]),int(split[4]))
    elif split[0] == "move":
        ans = movepos(ans,int(split[2]),int(split[5]))
    else:
        print "operation not found"
print "part1:%s"%ans

itr = range(len(total))[::-1]
ans = "fbgdceah"
for i in itr:
    split = total[i]
    if split[0] == "swap":
        if split[1] == "position":
            ans = swap_pos(ans,int(split[2]),int(split[5]))
        else:
            ans = swap_letter(ans,split[2],split[5])
    elif split[0] == "rotate":
        if split[1] == "based":
            copy = ans[:]
            for j in range(len(ans)):
                copy = rot(copy,"right",1)
                if rotpos(copy,split[6]) == ans:
                    ans = copy
                    break
        else:
            d = ("left" if (split[1] != "left") else "right")
            ans = rot(ans,d,int(split[2]))
    elif split[0] == "reverse":
        ans = reversepos(ans,int(split[2]),int(split[4]))
    elif split[0] == "move":
        ans = movepos(ans,int(split[5]),int(split[2]))
    else:
        print "operation not found"
print "part2:%s"%ans
