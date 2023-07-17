#..--...--..--
#https://stellasia.github.io/blog/2018-12-03-coding-exercice-morse/
import matplotlib.pyplot as plt
import re

def judge(x, y):
	inclination = []
	differ = 10
	for index, x_ele in enumerate(x):
		y_ele = y[index]
		#approximete
		if abs(y_ele) <= differ:
			inclination.append(0)
		elif y_ele > differ:
			inclination.append(1)
		else:
			inclination.append(-1)
	extract(inclination)
	print(inclination)

def extract(data):
	pattern = r'-1+0*1+'
	
	data_str = ''.join(str(num) for num in data)
	matches = re.findall(pattern, data_str)
	matches = [int(match) for match in matches]
	
	extract0_p = r'0+'
	extracted0 = []
	nem_extracted0 = []
	for m in matches:
		count = 0
		v = re.findall(extract0_p, str(m))
		extracted0.append(v)
		for v2 in v:
			for char in v2:
				count += 1
		nem_extracted0.append(count)
	print(extracted0)
	print(nem_extracted0)
	
def converge_to_zero(initial_value, convergence_rate):
	current_value = initial_value
	current_value -= current_value * convergence_rate

	return current_value

def noisefilter(previous, current):
	difference = abs(previous - current) 
	if difference > 3 and abs(current) < 50:
		k = 1
	else:
		k = 0
	return converge_to_zero(current, k)
	
def smooth_data(data, window_size):
	cumulative_sum = [0] * len(data)
	smoothed_data = [0] * len(data)

	cumulative_sum[0] = data[0]
	for i in range(1, len(data)):
		cumulative_sum[i] = cumulative_sum[i-1] + data[i]

	half_window = window_size // 2
	for i in range(len(data)):
		start_index = max(0, i - half_window)
		end_index = min(len(data) - 1, i + half_window)
		window_size_actual = end_index - start_index + 1
		smoothed_data[i] = int((cumulative_sum[end_index] - cumulative_sum[start_index] + data[start_index]) / window_size_actual)
		
	return smoothed_data

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
	interval = 2
	current_data = interval
	while current_data <= len(data):
		x_range = interval
		y_range = (int(data[current_data]) - int(data[current_data - interval]))
		
		#remake later
		previous_y = int(data[current_data - interval]) - int(data[current_data - interval*2])
		
		current_y = y_range
		new_y_range = noisefilter(previous_y, current_y)
		x.append(current_data)
		y.append(new_y_range)
		old_y.append(current_y)

		current_data += interval
	
	smooth_y = smooth_data(old_y, 5)
	print(smooth_y)
	judge(x, smooth_y)
	
	plt.plot(x, old_y)
	#plt.plot(x, smooth_y)
	makeplot(x, smooth_y)

def main2():
	file_path = "C:\\Users\\kiho\\OneDrive\\デスクトップ\\morse\\kiho2.txt"
	file = open(file_path, 'r')
	data = file.read().split()
	file.close()
	x = []
	y = []
	current_data = 0
	for _ in range(449):
		
		print(current_data)
		x.append(current_data)
		y.append(int(data[current_data]))

		current_data += 1
	
	makeplot(x, y)
	
if __name__ == '__main__':
	main()