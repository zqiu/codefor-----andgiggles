file = open("input")
input = file.readline()

def recparseinput(start,end,multiplier):
#    print "calling %d %d %d"%(start,end,multiplier)
    j = start
    toret = 0
    while j < end:
        index = input.find("(",j)
        if index == -1 or index >= end:
            toret += (end - j)
            j = end
        else:
            toret += (index - j)
            index2 = input.find(")",index)
            if index2 == -1 or index >= end:
                toret += (end - index)
                j = end
            else:
                l = input[index+1:index2].split("x")
                if (int(l[0]) + index2) >= end:
                    l[0] = end - (index2) - 1
                toret += recparseinput(index2+1,index2+int(l[0])+1,int(l[1]))
                j = index2 + int(l[0]) + 1
    return multiplier * toret
    
i = 0
ans = 0
ans2 = 0
while i < len(input):
    index = input.find("(",i)
    if index == -1:
        ans += (len(input) - i)
        ans2 += (len(input) - i)
        i = len(input)
    else:
        ans += (index - i)
        ans2 += (index - i)
        index2 = input.find(")",index)
        if index2 == -1:
            ans += (len(input) - index)
            ans2 += (len(input) - index)
            i = len(input)
        else:
            l = input[index+1:index2].split("x")
            if (int(l[0]) + index2) >= len(input):
                l[0] = len(input) - (index2) - 1
            ans += int(l[0]) * int(l[1])
            ans2 += recparseinput(index2+1,index2+int(l[0])+1,int(l[1]))
            i = index2 + int(l[0]) + 1
print "part one:" + str(ans)
print "part two:" + str(ans2)
