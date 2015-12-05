hi = open("input")

ans1 = 0

counter = 0
ans2 = 0

for line in hi:
    for char in line:
        counter += 1
        if char == '(':
            ans1 += 1
        elif char == ')':
            ans1 -= 1
        if ans1 == -1 and ans2 == 0:
            ans2 = counter

print ans1
print ans2
