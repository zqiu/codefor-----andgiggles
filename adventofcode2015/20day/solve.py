products=set()
input = 2900000
num = 1

def setsum(myset):
    val = 0
    for hi in myset:
        val += hi
    return val

def findall(indexlist,maxnums,anslist):
    findallr(indexlist,0,0,maxnums,1,anslist)

def findallr(indexlist,currentindex,currentnums,maxnums,ans,anslist):
    if currentnums == maxnums:
        anslist.append(ans)
        return
    if currentindex >= len(indexlist):
        return
    #don't include the surrent num in product
    findallr(indexlist,currentindex+1,currentnums,maxnums,ans,anslist)
    #include num
    findallr(indexlist,currentindex+1,currentnums+1,maxnums,ans*indexlist[currentindex],anslist)

a = list()
findall(range(1,10),9,a)
#print a

#while setsum(products) < input:
while num < 10:
    #print num
    for i in range(1,num):
        plist = list()
        findall(range(1,num),i,plist)
        for hi in plist:
            products.add(hi)
    num += 1
    #print num
print products
print max(products)
