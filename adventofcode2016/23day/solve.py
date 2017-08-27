ops = []
f = open("input")
registers = [7, 0, 0, 0]
#for part 2
#registers = [12, 0, 0, 0]
encoding = "abcd"

#0 = cpy, 1 = inc, 2 = dec, 3 = jnz, 4 = tgl, 5 = mul
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
    elif chunked[0] == "tgl":
        ops.append((4, chunked[1]))
    elif chunked[0] == "mul":
        ops.append((5, chunked[1], chunked[2]))

def do():
    current = 0
    while current < len(ops):
        #parse ops
        instruct = ops[current]
        if instruct[0] == 0:
            reg = encoding.find(instruct[1])
            dest = encoding.find(instruct[2])
            if dest != -1:
                if reg != -1:
                    registers[dest] = registers[reg]
                else:
                    registers[dest] = int(instruct[1])
        elif instruct[0] == 1 and encoding.find(instruct[1]) != -1:
            registers[encoding.find(instruct[1])] += 1
        elif instruct[0] == 2 and encoding.find(instruct[1]) != -1:
            registers[encoding.find(instruct[1])] -= 1
        elif instruct[0] == 4:
            tochange = registers[encoding.find(instruct[1])]
            if tochange != 0:
                tochangeaddr = current+tochange
                if tochangeaddr < len(ops):
                    tochangeop = ops[tochangeaddr]
                    if tochangeop[0] == 0:
                        ops[tochangeaddr] = ((3, tochangeop[1], tochangeop[2]))
                    elif tochangeop[0] == 3:
                        ops[tochangeaddr] = ((0, tochangeop[1], tochangeop[2]))
                    elif tochangeop[0] == 1:
                        ops[tochangeaddr] = ((2, tochangeop[1]))
                    else:
                        ops[tochangeaddr] = ((1, tochangeop[1]))
        elif instruct[0] == 5:
            registers[0] += registers[encoding.find(instruct[1])] * registers[encoding.find(instruct[2])]
        else:
            val = instruct[1]
            reg = encoding.find(val)
            if reg != -1:
                val = registers[reg]
            tomove = instruct[2]
            if encoding.find(tomove) != -1:
                tomove = registers[encoding.find(tomove)]
            if int(val) != 0:
                current += int(tomove)
            else:
                current += 1
        if instruct[0] != 3:
            current += 1
            
do()
print "answer:%d"%registers[0]
