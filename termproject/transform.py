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
      regex2 = re.compile(',')
      line2 = re.sub(r'"(\d*),(\d*)"', r'\1\2', line)
      line3 = re.sub(r',', r'\t', line2)
      line4 = re.sub(r'(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)', r'\1\t\2\t\4\t\5', line3)
      line5 = re.sub(r'/',r'', line4)
      OUTPUTFILE.write(line5)
  FILE.close()
  OUTPUTFILE.close()
