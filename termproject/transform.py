# requires the geopy library from this site:
# https://code.google.com/p/geopy/
# only runs under python 2.6 because of geopy
import sys
import re
from geopy import geocoders

for file in sys.argv[1:]:
  print "working on " + file + "..."
  FILE = open(file)
  outfile = file.replace('csv','data')
  OUTPUTFILE = open(outfile, "w")

  # get the coords automatically by lat/longing the filename + "AL".
  place = file.replace('.csv','').replace('_', ' ') + ', AL'
  place = re.sub(r'(.*)/(.*)',r'\2', place)
  g = geocoders.Google()
  placeCoords, (lat, lng) = g.geocode(place)
  # add the coordinates
  outputcoords = str(lat) + " " + str(lng) + "\n"
  OUTPUTFILE.write(outputcoords)

  firstLine = True
  for line in FILE:
    if firstLine:
      firstLine = False
    else:
      # cleans up the input. Reformats the data field, removes unused averages,
      # and cleans up any weird formatting artifacts
      line = re.sub(r'(\d*)/(\d*)/(\d*)',r'\1\3', re.sub(r'(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)\t(.*)', r'\1\t\2\t\4\t\5', re.sub(r',', r'\t', re.sub(r'"(\d*),(\d*)"', r'\1\2', line))))
      OUTPUTFILE.write(line)
  FILE.close()
  OUTPUTFILE.close()
