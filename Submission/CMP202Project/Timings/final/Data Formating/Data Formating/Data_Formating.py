import re
import csv

filename = "generation in work 1592595183"

file = open(filename+".csv","r")
newFile = open(filename + " formated.csv","w", newline = "")

reader = csv.reader(file, delimiter=',')
writer = csv.writer(newFile, delimiter=',')

for row in reader:
    data = row
    break # read one line only

for l in data:
    writer.writerow([l])

# =(A1:A50000/50000)*1000000000

file.close();
newFile.close();