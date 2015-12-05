coords = set([(0,0)])

s_coords = set([(0,0)])
r_coords = set([(0,0)])

x = 0
y = 0

s_x = 0
s_y = 0
r_x = 0
r_y = 0
alt = 0

file = open("input")

for line in file:
    for char in line:
        if char == '<':
            #print "left"
            x -= 1
            if alt == 0:
                s_x -= 1
            else:
                r_x -= 1
        elif char == '>':
            #print "right"
            x += 1
            if alt == 0:
                s_x += 1
            else:
                r_x += 1
        elif char == '^':
            #print "up"
            y += 1
            if alt == 0:
                s_y += 1
            else:
                r_y += 1
        elif char == 'v':
            #print "down"
            y -= 1
            if alt == 0:
                s_y -= 1
            else:
                r_y -= 1
        coords.add((x,y))
        if alt == 0:
            s_coords.add((s_x,s_y))
        else:
            r_coords.add((r_x,r_y))
        alt = (alt + 1) % 2

print len(coords)
print len(s_coords.union(r_coords))
