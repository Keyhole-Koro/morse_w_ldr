import time
import array
import datetime

def main(file):
	data = file.read().split()
	l_input_data = []
	input_data = array.array('i')
	count = 0
	start_time = datetime.datetime()
	function1Interval = 1000
	function2Interval = 1000
	
	function1PreviousMillis = convert_millis(current_time)
	function2PreviousMillis = convert_millis(current_time)
	
	for d in data:#equivalent to loop
		elapsed_time = elapsed(start_time)#in milliseconds which is equivalent to millis in arduino
		
		if elapsed_time - function1PreviousMillis  >= function1Interval:#get values from ldr
			current_time = datetime.datetime()
			function1PreviousMillis = convert_millis(current_time)
			set_value(d)
			
		if elapsed_time - function2PreviousMillis  >= function2Interval:#calculus
			current_time = datetime.datetime()
			function2PreviousMillis = convert_millis(current_time)
			
			
		time.sleep(0.005)

def set_value(value):
	input_data.append(value)
	if count == 200:
		l_input_data.append(input_data)
		del input_data
		input_data = array.array('i')
		count = 0
	count++1

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
	