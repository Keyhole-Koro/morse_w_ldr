import time
import array
import datetime

count = 0

def main(file):
	data = file.read().split()
	l_input_data = []
	input_data = array.array('i')
	count = 0
	start_time = time.time()
	function1Interval = 20
	function2Interval = 1000
	
	stored_values = {'current_inclination':0,'previous_inclination':0}
	
	function1PreviousMillis = convert_millis(start_time)
	function2PreviousMillis = convert_millis(start_time)
	#equivalent to loop
	index = 0
	while True:
		d = data[index]
		#in milliseconds which is equivalent to millis in arduino
		elapsed_time = elapsed(start_time)
		print(index)
		print(function1PreviousMillis, elapsed_time)
		#get values from ldr
		if function1PreviousMillis - elapsed_time >= function1Interval:
			seconds = time.time()
			function1PreviousMillis = convert_millis(seconds)
			
			previous_ele_num = max(0, count-5)#it doesnt include [0-4] i think
			
			new_value = int(d)
			
			if previous_ele_num == 0:
				previous_value = 0
				new_inclination = int(d)
			else:
				print(new_value, previous_value)
				new_inclination = inclination(2, new_value - previous_value)
				
			stored_values['current_inclination'] = new_inclination
			
			mean_value = int(smooth_data(stored_values))
			print(mean_value)
			
			input_data.append(mean_value)
			if count == 200:
				l_input_data.append(input_data)
				del input_data
				input_data = array.array('i')
				count = 0
			count++1
			
			previous_inclination = mean_value
			
			function2PreviousMillis = convert_millis(seconds)
			
			index++1

		if function2PreviousMillis - elapsed_time >= function2Interval:#calculus
			seconds2 = time.time()
			
		time.sleep(0.01)
			
			
def set_value(input_data, value):
	input_data.append(value)
	if count == 200:#200/2
		l_input_data.append(input_data)
		del input_data
		input_data = array.array('i')
		count = 0
	count++1
	return input_data
	
def inclination(x_range, y_range):
	value = y_range/x_range
	return value

def set_mean_values():
	pass

def mean_values(new_value, previous_value):
	new_sum = new_value + previous_value
	near_sum = new_sum/2
	return near_sum
	
	
def elapsed(start_time):
	now = time.time()
	now_milli = convert_millis(now)
	start_milli = convert_millis(start_time)
	between_milli = now_milli - start_milli
	#print(now_milli, start_milli, between_milli)
	millis = between_milli
	return millis

def convert_millis(seconds):
	milliseconds = round(seconds*1000)
	return milliseconds

def smooth_data(stored_values):
	new_value = stored_values['current_inclination']
	previous_value = stored_values['previous_inclination']
	
	mean_value = mean_values(new_value, previous_value)
	
	return mean_value

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
	