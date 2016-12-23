ops = []
f = open("input")
registers = [0, 0, 0, 0]
encoding = "abcd"

#0 = cpy, 1 = inc, 2 = dec, 3 = jnz
for line in f:
    chunked = line[:-1].split(" ")
    if chunked[0] == "cpy":
        ops.append((0, chunked[1], chunked[2]))
    elif chunked[0] == "inc":
        ops.append((1, chunked[1]))
    elif chunked[0] == "dec":
        ops.append((2, chunked[1]))
    elif chunked[0] == "jnz":
        ops.append((3, chunked[1], chunked[2]))

def do():
    current = 0
    while current < len(ops):
        #parse ops
        instruct = ops[current]
        if instruct[0] == 0:
            reg = encoding.find(instruct[1])
            dest = encoding.find(instruct[2])
            if reg != -1:
                registers[dest] = registers[reg]
            else:
                registers[dest] = int(instruct[1])
        elif instruct[0] == 1:
            registers[encoding.find(instruct[1])] += 1
        elif instruct[0] == 2:
            registers[encoding.find(instruct[1])] -= 1
        else:
            val = instruct[1]
            reg = encoding.find(val)
            if reg != -1:
                val = registers[reg]
            if int(val) != 0:
                current += int(instruct[2])
            else:
                current += 1
        if instruct[0] != 3:
            current += 1

do()
print "part1:%d"%registers[0]
registers = [0, 0, 1, 0]
do()
print "part2:%d"%registers[0]