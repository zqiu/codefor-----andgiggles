#part 1
#INPUT = [(13,10),(17,15),(19,17),(7,1),(5,0),(3,1)]
#part 2
INPUT = [(13,10),(17,15),(19,17),(7,1),(5,0),(3,1),(11,0)]

ANS = 0
JUMP = 1
for i in range(len(INPUT)):
    a = INPUT[i]
    while((ANS + a[1] + i + 1) % a[0] != 0):
        ANS += JUMP
    JUMP *= a[0]
    
print ANS
