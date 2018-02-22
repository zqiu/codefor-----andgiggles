import sys
if len(sys.argv) != 3:
	print "need in and out file"
	sys.exit()
f = open(sys.argv[1])
f2 = open(sys.argv[2],"w")
songs = []
artist = ""

def capitalize(s):
	return " ".join(cap(w) for w in s.split(" "))

def cap(s):
	firstchar = 0
	for i in range(len(s)):
		if s[i].isalnum():
			firstchar = i
			break
	if firstchar == 0:
		return s.capitalize()
	else:
		return s[0]+s[i:].capitalize()

def isempty(s):
	for char in s:
		if char.isalnum():
			return False
	return True
		
for line in f:
	if isempty(line):
		continue
	elif line[0] == '\t':
		songs.append(capitalize(line))
	else:
		if artist != "":
			songs.sort()
			f2.write(capitalize(artist))
			for song in songs:
				f2.write(song)
		artist = line
		songs = []
songs.sort()
f2.write(capitalize(artist))
for song in songs:
	f2.write(song)
