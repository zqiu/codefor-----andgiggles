#/usr/bin/env python2
"""
Create new file based on byte-by-byte manipulation of another.

Can be used for:
- file carving
- xor-ing files
- steganography
"""

from sys import argv

if len(argv) < 2:
	print "Usage:", argv[0], "input_filename [output_filename]"
	exit(1)
if len(argv) < 3:
	argv.append(argv[1] + ".out")

with open(argv[1], "rb") as inFile, open(argv[2], "wb") as outFile:
	buf = inFile.read()
	for i in range(len(buf))
		# Manipulate here
		c = buf[i]
		outFile.write(c)
