INPUT = 3017957
gifts = range(1,INPUT+1)

start = 0
while len(gifts) > 1:
    tmp = (start+len(gifts)%2)%2
    gifts = gifts[start::2]
    start = tmp
print "ans1:%d"%gifts[0]

gifts = range(1,INPUT+1)
while len(gifts) > 1:
    removed = 0
    toremove = len(gifts)/2
    for i in range(len(gifts)):
        if gifts[i] == -1:
            continue
        removed += 1
        gifts[toremove] = -1
        tojump = 0
        if (len(gifts) - removed)%2 == 0:
            tojump = 2
        else:
            tojump = 1
        itr = 0
        while itr != tojump:
            toremove = (toremove + 1)%len(gifts)
            if gifts[toremove] != -1:
                itr += 1
    tmp = []
    for i in gifts:
        if i != -1:
            tmp.append(i)
    gifts = tmp
print "ans2:%d"%gifts[0]
