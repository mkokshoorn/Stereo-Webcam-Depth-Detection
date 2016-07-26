import os
import csv







value_list=[]

x=[]
y=[]
z=[]

#3DPROJ
'''
raw_data_file = open("SavedDispMap.txt",'r')


for i in range(5):
	raw_data_file.readline()


line=raw_data_file.readline()
values=line[10:].split(',')
raw_values=values[:-1]


for raw_value in raw_values:
	print raw_value
	if(not(raw_value==' 10000.')):
		temp=float(raw_value[:10])*(10**int(raw_value[12:]))
		value_list.append(temp)

	else:
		value_list.append(1000.0)



lines = raw_data_file.readlines()
lines = [line.split(',') for line in lines]

raw_data=[]
for line in lines:
	if(len(line)>=1):
		raw_data.append(line[0])
	if(len(line)>=2):
		raw_data.append(line[1])
	if(len(line)>=3):
		raw_data.append(line[2])
	if(len(line)>=4):
		raw_data.append(line[3])
	if(len(line)>=5):
		raw_data.append(line[4])






raw_data[-1]=raw_data[-1][:-3]




for raw_value in raw_data:
	if(not(raw_value=='\n')):
		if((raw_value==' 10000.')or (raw_value=='       10000.') or (raw_value==' 10000. ')):
			value_list.append(10000.0)


		else:
			temp=float(raw_value[:-5])*(10**int(raw_value[-4:]))
			value_list.append(temp)



for i in range(0,len(value_list)-1,3):
	if(1):#not(value_list[i+2])==10000.0):
		x.append(value_list[i  ])
		y.append(value_list[i+1])
		z.append(value_list[i+2])




print len(value_list)
print len(x)
print len(y)
print len(z)





import csv



writer = csv.writer(open("x.csv", 'w'))
writer.writerow(x)

writer = csv.writer(open("y.csv", 'w'))
writer.writerow(y)
writer = csv.writer(open("z.csv", 'w'))
writer.writerow(z)


writer = csv.writer(open("all.csv", 'w'))
writer.writerow(value_list)

'''




#DISP
raw_data_file = open("SavedDispMap.txt",'r')


for i in range(5):
	raw_data_file.readline()


line=raw_data_file.readline()
values=line[10:].split(',')
raw_values=values[:-1]


for raw_value in raw_values:
	if(not(raw_value==' 10000.')):
		temp=float(raw_value)
		value_list.append(temp)

	else:
		value_list.append(1000.0)



lines = raw_data_file.readlines()
lines = [line.split(',') for line in lines]

raw_data=[]
for line in lines:
	if(len(line)>=1):
		raw_data.append(line[0])
	if(len(line)>=2):
		raw_data.append(line[1])
	if(len(line)>=3):
		raw_data.append(line[2])
	if(len(line)>=4):
		raw_data.append(line[3])
	if(len(line)>=5):
		raw_data.append(line[4])
	if(len(line)>=6):
		raw_data.append(line[5])
	if(len(line)>=7):
		raw_data.append(line[6])
	if(len(line)>=8):
		raw_data.append(line[7])
	if(len(line)>=9):
		raw_data.append(line[8])
	if(len(line)>=10):
		raw_data.append(line[9])					
	if(len(line)>=11):
		raw_data.append(line[10])	
	if(len(line)>=12):
		raw_data.append(line[11])	
	if(len(line)>=13):
		raw_data.append(line[12])	



raw_data[-1]=raw_data[-1][:-3]


index=0

for raw_value in raw_data:
	if(not(raw_value=='\n')):
		if((raw_value==' 10000.')or (raw_value=='       10000.') or (raw_value==' 10000. ')):
			value_list.append(800.0)



		else:
			temp=float(raw_value)
			value_list.append(temp)




print len(value_list)
import csv






writer = csv.writer(open("all.csv", 'w'))
writer.writerow(value_list)

