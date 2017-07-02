import csv,sys

headers = []
column = {}

with open('outputs.csv', 'r') as f:
  reader = csv.reader(f)
  headers = next(reader, None)
  column = {}
  for h in headers:
    column[h] = []
  for row in reader:
    for h, v in zip(headers, row):
      column[h].append(v)

l = '{sensorFMU}.sensor1.lf_1_sensor_reading'
r = '{sensorFMU}.sensor2.lf_1_sensor_reading'
    
output = []    
for i in range(len(column[headers[0]])):
    output.append("mk_(%i,%i)" % (int(round(float(column[l][i]))),int(round(float(column[r][i])))))

with open("lf.txt", 'w', newline='\n') as f:
      f.write("[")
      f.write(",\n".join(output[1:51]))
      f.write("]")
#print(column[headers[0]])