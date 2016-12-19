import md5

doorid = "wtnhxymk"
ans = ""
current = 0
ans2 = "XXXXXXXX"
solved = 0

m = md5.new(doorid)
while solved < 8:
    c = m.copy()
    c.update(str(current))
    if c.hexdigest()[0:5] == "00000":
        if len(ans) < 8:
            ans += c.hexdigest()[5]
#        print c.hexdigest()
        if (ord(c.hexdigest()[5]) - ord('0')) < 8:
            index = int(c.hexdigest()[5])
            if ans2[index] == 'X':
                ans2 = ans2[:index] + c.hexdigest()[6] + ans2[index+1:]
                solved += 1
    current += 1
        
print "part1:" + ans
print "part2:" + ans2
