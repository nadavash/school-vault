import sys, random, re

RANGE_MAX = 0

if len(sys.argv) != 3:
	sys.stderr.write("Usage: ./filter_page_counts.py [input file] [output file]\n")
	exit()

infile  = open(sys.argv[1], "r", encoding="utf8")
outfile = open(sys.argv[2], "w", encoding="utf8", newline='\n')

regex = re.compile("^[a-zA-Z_]+$")
for line in infile:
	parts = line.split(" ")
	if parts[0] == "en" and regex.match(parts[1]):
		outfile.write(parts[1].replace("_", " ").lower())
		outfile.write(":")
		outfile.write(parts[2])
		outfile.write("\n")

infile.close()
outfile.close()