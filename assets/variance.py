import math

def readData(data):
	array = []
	filename = open(data,'r')	
   	for lines in filename.read().split(' '):
		array.append(lines.split(' '))
	filename.close()
	return array[:]

def fixData(data):
	array = []
	for i in range(len(data)):
		for j in range(len(data[i])):
			array.append(int(data[i][j]))
	return array

def calcMean(data):
	total = 0
	for num in data:
		total += num
	mean = total / len(data)
	return mean

def calcVariance(data,mean):
	variance = 0
	tmp = 0
	total = 0
	for num in data:
		tmp = math.pow(mean - num, 2)
		total += tmp
	variance = total / len(data)
	return variance

def Variance():
	mv = []
	mean = []
	variance = []
	string = ["10cm.txt","20cm.txt","30cm.txt","40cm.txt"]
	for i in range(len(string)):
		data = readData(string[i])
		data = fixData(data)
		mean.append(calcMean(data))
		variance.append(calcVariance(data,mean[i]))
		mv.append((mean[i],variance[i]))
	return mv

def calcXY(filename):
	data = readData(filename)
	data = fixData(data) 
	

def main():
	mean = []
	variance = []
	mv = Variance()
	print mv
	calcXY("10cm.txt")


if __name__=="__main__":
	main()


