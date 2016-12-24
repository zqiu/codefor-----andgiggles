import md5

part2 = True

def issameblock(inval,blocklen):
    if len(inval) <  blocklen:
        return -1
    for i in range(len(inval) - blocklen + 1):
        val = inval[i]
        for j in range(blocklen):
            if val != inval[j + i]:
                break
            if j == (blocklen - 1):
                return val
    return -1

def tryval(index,fives):
    global datasets
    copy = set()
    for a in datasets:
        if (a[0] + 1000) >= index:
            copy.add(a)
    datasets = copy
    for a in datasets:
        if a[1] == fives and a[0] != index:
            answers.append(a[0])

def generatehash(index):
    tmp = m.copy()
    tmp.update(str(index))
    if part2:
        for i in range(2016):
            tmp2 = md5.new(tmp.hexdigest())
            tmp = tmp2
    return tmp.hexdigest()

puzzleinput = "ihaygndm"
#puzzleinput = "abc"
datasets = set()
answers = []
start = 0
m = md5.new(puzzleinput)

while len(answers) < 64:
    digest = generatehash(start)
    triples = issameblock(digest,3)
    fives = issameblock(digest,5)
    if triples != -1:
        datasets.add((start,triples))
    if fives != -1:
        tryval(start,fives)
    start += 1
    if start%5000 == 0:
        print "calculated %d"%start

maxtotry = sorted(datasets,reverse = True)[0][0]
for i in range(start,maxtotry+1000):
    digest = generatehash(i)
    fives = issameblock(digest,5)
    if fives != -1:
        tryval(i, fives)

answers = sorted(answers)
print "answer:%d"%answers[63]
