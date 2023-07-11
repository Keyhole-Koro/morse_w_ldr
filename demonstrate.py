import time
import array
import datetime

previous_y_value = 0

def main(file):
	data = file.read().split()
	l_input_data = []
	input_data = array.array('i')
	count = 0
	start_time = datetime.datetime()
	function1Interval = 20
	function2Interval = 1000
	
	function1PreviousMillis = convert_millis(current_time)
	function2PreviousMillis = convert_millis(current_time)
	#equivalent to loop
	for d in data:
		#in milliseconds which is equivalent to millis in arduino
		elapsed_time = elapsed(start_time)
		
		#get values from ldr
		if elapsed_time - function1PreviousMillis  >= function1Interval:
			current_time = datetime.datetime()
			function1PreviousMillis = convert_millis(current_time)
			if previous_ele_num == 0:
				previous_y_value = d
			set_value(d, previous_y_value)
			
		if elapsed_time - function2PreviousMillis  >= function2Interval:#calculus
			current_time = datetime.datetime()
			function2PreviousMillis = convert_millis(current_time)
			smooth_data(input_data, 5)
			
def set_value(new_y_value, previous_y_value):
	x_range = 2
	y_range = new_y_value - previous_y_value
	inclination_value = int(inclination(x_range, y_range))
	input_data.append(inclination_value)
	if count == 200:#200/2
		l_input_data.append(input_data)
		del input_data
		input_data = array.array('i')
		count = 0
	previous_y_value = new_y_value
	count++1
	
def inclination(x_range, y_range):
	value = y_range/x_range
	return value

def set_mean_values():
	pass

def mean_values(near_sum, new_value, previous_value):
	difference = new_value - previous_value
	new_sum = near_sum + difference
	near_sum = new_sum/2
	return near_sum
	
	
def elapsed(time):
	between = datetime.datetime()-time
	millis = convert_milli(between)
	return millis

def convert_millis(time):
	seconds = time.total_seconds()
	millis = round(seconds*1000)
	return millis

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
	
if __name__ == '__main__':
	file_path = "C:\\Users\\kiho\\OneDrive\\デスクトップ\\morse\\KihoData.txt"
	file = open(file_path, 'r')
	main(file)
	