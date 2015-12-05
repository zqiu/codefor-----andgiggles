file = open("input")

total = 0
total2 = 0
for line in file:
    dimensions = line.strip('\n').split('x')
    for i in range(len(dimensions)):
        dimensions[i] = int(dimensions[i])
    if len(dimensions) == 3:
        area = []
        area.append(dimensions[0] * dimensions[1])
        area.append(dimensions[0] * dimensions[2])
        area.append(dimensions[1] * dimensions[2])
        for surface in area:
            total += 2*surface
        total += min(area)
        temp = 0
        volume = 1
        for surface in dimensions:
            temp += surface
            volume *= surface
        temp -= max(dimensions)
        total2 = total2 + 2*temp + volume

print total
print total2
