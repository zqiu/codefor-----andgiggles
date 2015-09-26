import subprocess
import time

#password's dr4g0n_or_p4tric1an_it5_LLVM

alpha = "0123456789abcdef"

def inttohex(intnum):
    return "\\x" + alpha[intnum/16] + alpha[intnum%16] 

p = subprocess.Popen("gdb wyvern_c85f1be480808a9da350faaa6104a19b -n", 
                     stdin = subprocess.PIPE,
                     stdout = subprocess.PIPE,
                     stderr = subprocess.STDOUT,
                     shell = True)

p.stdin.write("b *0x0000040e25a\n")
p.stdin.write("set pagination off\n")

#for i in range(32,127):
for i in range(99,127):
    p.stdin.write("r < <(python -c \"print '" + inttohex(i) + "'*28 + '\\n' \")\n")
    p.stdin.write("set $count=0\n")
    p.stdin.write("while ($pc != 0x0000040e266)\n")
    p.stdin.write("stepi 10\n")
    p.stdin.write("set $count=1+$count\n")
    p.stdin.write("end\n")
    p.stdin.write("print $count\n")
    while True:
        output =  p.stdout.readline()
        if(output[:7] == "(gdb) $"):
            print str(i) + output[:-1]
            break
