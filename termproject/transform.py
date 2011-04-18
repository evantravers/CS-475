import sys
import re

for file in sys.argv[1:]:
  FILE = open(file)
  OUTPUTFILE = open(file+"b", "w")
  for line in FILE:
    # take out all the "\d,\d", replace with $1$2
    regex2 = re.compile(',')
    line2 = re.sub(r'"(\d*),(\d*)"', r'\1\2', line)
    line3 = re.sub(r',', r'\t', line2)
    OUTPUTFILE.write(line3)
    print(line3)
  FILE.close()
  OUTPUTFILE.close()
