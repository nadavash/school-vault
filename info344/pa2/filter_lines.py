import sys, random, re

RANGE_MAX = 4

if len(sys.argv) != 3:
	sys.stderr.write("Usage: ./filter_lines.py [input file] [output file]\n")
	exit()

infile  = open(sys.argv[1], "r", encoding="utf8")
outfile = open(sys.argv[2], "w", encoding="utf8", newline='\n')

lineSkip = 1
regex = re.compile("^[a-zA-Z_]+$")
for line in infile:
	if lineSkip > 0:
		lineSkip -= 1
		continue
	if regex.match(line) and random.randint(0,RANGE_MAX) == 0:
		outfile.write(line.replace("_", " ").lower())

infile.close()
outfile.close()