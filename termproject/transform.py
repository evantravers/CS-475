import sys
import re

# TODO get the coords automatically by lat/longing the filename + "AL".
for file in sys.argv[1:]:
  FILE = open(file)
  outfile = file.replace('csv','data')
  OUTPUTFILE = open(outfile, "w")
  firstLine = True
  for line in FILE:
    if firstLine:
      firstLine = False
    else:
      # take out all the "\d,\d", replace with $1$2
      line = re.sub(r'(\d*)/(\d*)/(\d*)',r'\1\3', re.sub(r'(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)', r'\1\t\2\t\4\t\5', re.sub(r',', r'\t', re.sub(r'"(\d*),(\d*)"', r'\1\2', line))))
      OUTPUTFILE.write(line)
  FILE.close()
  OUTPUTFILE.close()
