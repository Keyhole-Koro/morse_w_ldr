import time
import array
import datetime
import threading#is not implemented in arduino
import matplotlib.pyplot as plt


class morse:
	"""value is single
	data is multiple like list array
	(in most cases)"""
	def __init__(self, file):
		self.file = file
		self.data = self.file.read().split()
		
		self.l_input_data = []
		self.count = 0
		self.input_data = array.array('i')
		self.index = 0
		self.mean_input_data = array.array('i')
		
	def main(self):
		start_time = time.time()
		function1Interval = 20
		function2Interval = 100
		function3Interval = 10000
		
		function1PreviousMillis = 0
		function2PreviousMillis = 0
		function3PreviousMillis = 0
		
		previous_value = None
		
		data_index = 0
		
		all_data = []#temporary
		
		while True:
			#in milliseconds which is equivalent to millis in arduino
			elapsed_time = elapsed(start_time)
			
			if elapsed_time - function1PreviousMillis >= function1Interval:
				function1PreviousMillis = elapsed_time
				
				###
				new_input_value = int(self.data[data_index])
				
				if previous_value == None:
					previous_value = new_input_value
				
				inclination = (new_input_value - previous_value)/2
				
				previous_value = new_input_value
				###
				
				if self.index == 50:
					smoothed_data = smooth_data(self.input_data, 5)
					del self.input_data
					self.l_input_data.append(smoothed_data)
					self.input_data = array.array('i')
					self.index = 0
				self.input_data.append(round(inclination))
				self.index += 1
				data_index += 1#not len

			if elapsed_time - function2PreviousMillis >= function2Interval:#add or extract_data < 
				function2PreviousMillis = elapsed_time
				
				if len(self.l_input_data) > 0:
					all_data.extend(self.l_input_data[0])
					del self.l_input_data[0]
				
			if elapsed_time - function3PreviousMillis >= function3Interval:
				plt.plot(range(len(all_data)), all_data)
				plt.show()
			
			
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

def converge_to_zero(current_value, convergence_rate):
	current_value -= current_value * convergence_rate
	return current_value

def elapsed(start_time):
	now = time.time()
	between = now - start_time
	'''now_milli = convert_millis(now)
	start_milli = convert_millis(start_time)
	between_milli = now_milli - start_milli
	millis = between_milli'''
	#print(now_milli, start_milli, between_milli)
	millis = convert_millis(between)
	return millis

def convert_millis(seconds):
	milliseconds = round(seconds*1000)
	return milliseconds

if __name__ == '__main__':
	file_path = "C:\\Users\\kiho\\OneDrive\\デスクトップ\\morse\\KihoData.txt"
	file = open(file_path, 'r')
	morse(file).main()
