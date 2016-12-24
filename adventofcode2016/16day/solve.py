TOFILL = 272
#PART 2
#TOFILL = 35651584
INPUT = "11101000110010100"

while len(INPUT) < TOFILL:
    reverse = INPUT[::-1]
    INPUT += "0"
    for char in reverse:
        if char == "0":
            INPUT += "1"
        else:
            INPUT += "0"

INPUT = INPUT[:TOFILL]
while len(INPUT)%2 == 0:
    tmp = ""
    for i in range(len(INPUT)/2):
        if INPUT[i*2] == INPUT[i*2+1]:
            tmp += "1"
        else:
            tmp += "0"
    INPUT = tmp

print "answer:"+INPUT
