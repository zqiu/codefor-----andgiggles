import math
from itertools import count
from collections import defaultdict

def iscorner(boxnum,test):
	if (test - boxnum + 2) % (boxnum - 1) == 0:
		return True
	return False
input = 368078
squarenum = 1
while squarenum * squarenum < input:
	squarenum += 2
remainder = squarenum * squarenum - input
while remainder > squarenum:
	remainder -= (squarenum - 1)
ans = (squarenum / 2) + int(math.fabs(remainder - squarenum/2))
print ans

#part2 from https://oeis.org/A141481/b141481.txt
#also alternative code below taken from take from 
#https://www.reddit.com/r/adventofcode/comments/7h7ufl/2017_day_3_solutions/
# def sum_spiral():
    # a, i, j = defaultdict(int), 0, 0
    # a[0,0] = 1
    # sn = lambda i,j: sum(a[k,l] for k in range(i-1,i+2)
                                # for l in range(j-1,j+2))
    # for s in count(1, 2):
        # for _ in range(s):   i += 1; a[i,j] = sn(i,j); yield a[i,j]
        # for _ in range(s):   j -= 1; a[i,j] = sn(i,j); yield a[i,j]
        # for _ in range(s+1): i -= 1; a[i,j] = sn(i,j); yield a[i,j]
        # for _ in range(s+1): j += 1; a[i,j] = sn(i,j); yield a[i,j]

# def part2(n):
    # for x in sum_spiral():
        # if x>n: return x

# print part2(input)