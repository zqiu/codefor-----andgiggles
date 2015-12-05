import md5

input = "iwrupvqb"
#input = "pqrstuv"
num = 0
ans1 = 0

while 1:
    hash = md5.new(input + str(num))
    digest = hash.digest()
    if ord(digest[0]) == 0 and ord(digest[1]) == 0 and ord(digest[2]) < 16:
        if ans1 == 0:
            ans1 = num
        if ord(digest[2]) == 0:
            ans2 = num
            break
    num += 1

print ans1
print ans2
