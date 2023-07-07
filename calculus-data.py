#..--...--..--
#https://stellasia.github.io/blog/2018-12-03-coding-exercice-morse/
import matplotlib.pyplot as plt

def judge(x, y):
	incliments = []
	differ = 10
	for index, x_ele in enumerate(x):
		y_ele = y[index]
		#approximete
		if abs(y_ele) <= differ:
			incliments.append(1)
		elif y_ele > differ:
			incliments.append(0)
		else:
			incliments.append(-1)
	print(incliments)

def converge_to_zero(initial_value, convergence_rate):
	current_value = initial_value

	#while abs(current_value) > 0.001:  # Specify a threshold for convergence
	current_value -= current_value * convergence_rate

	return current_value

def noisefilter(previous, current):
	difference = abs(previous - current) 
	if difference > 3 and abs(current) < 35:
		k = 0.9
		print(abs(previous - current))
	else:
		k = 0
	return converge_to_zero(current, k)
	#return current+k*(previous - current)

def makeplot(x, y):
	plt.plot(x, y)
	
	plt.xlabel('X Range')
	plt.ylabel('Y Range')
	plt.title('Plot of X Range and Y Range')

	plt.show()

def main():
	file_path = "C:\\Users\\kiho\\OneDrive\\デスクトップ\\morse\\KihoData.txt"
	file = open(file_path, 'r')
	data = file.read().split()
	file.close()
	x = []
	y = []
	old_y = []
	multiply = 3
	current_data = multiply
	while current_data <= len(data):
		x_range = multiply
		y_range = int(data[current_data]) - int(data[current_data - multiply])
		
		#remake later
		previous_y = int(data[current_data - multiply]) - int(data[current_data - multiply*2])
		
		current_y = y_range
		new_y_range = noisefilter(previous_y, current_y)
		x.append(current_data)
		y.append(new_y_range)
		old_y.append(current_y)

		#print(previous_y, current_y, y_range, new_y_range)
		current_data += multiply
	judge(x, y)
	plt.plot(x, old_y)
	makeplot(x, y)

main()


