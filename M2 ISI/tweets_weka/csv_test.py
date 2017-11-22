import csv





with open('polarimots_1.csv', mode='r') as infile:
    reader = csv.reader(infile)
    polarimots = dict((rows[0],rows[1:4]) for rows in reader)


s = "#ecologie militants dame landes, justice devient folle <url> abdomen"

for m in s.split(' '):
    if m in polarimots :
        print m