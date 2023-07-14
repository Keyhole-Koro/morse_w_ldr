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
		self.mean_data = array.array('i')
		self.mean_value_count = 0
		self.l_approximated_data = []
		
	def main(self):
		start_time = time.time()
		function1Interval = 20
		function2Interval = 1000
		function3Interval = 2000
		
		function1PreviousMillis = 0
		function2PreviousMillis = 0
		function3PreviousMillis = 0
		
		data_index = 0
		
		previous_value = int(self.data[0])#temporary
		
		extract_data = 0
		
		
		
		while True:
			#in milliseconds which is equivalent to millis in arduino
			elapsed_time = elapsed(start_time)
			
			if elapsed_time - function1PreviousMillis >= function1Interval:
				function1PreviousMillis = elapsed_time
				
				new_input_value = int(self.data[data_index])#get values from ldr
				self.set_value(int((new_input_value-previous_value)/2))
				
				smoothed_value = self.smooth_value()
				self.set_mean(smoothed_value)
				
				self.ifFull()
				
				previous_value = new_input_value
				
				data_index+=1

			if elapsed_time - function2PreviousMillis >= function2Interval:#add or extract_data < 
				function2PreviousMillis = elapsed_time
				
				data = self.l_input_data[extract_data]
				
				if data.buffer_info()[1] == 200:
					approximated_data = approximate(data)

					self.l_approximated_data.append(approximated_data)
					
					extract_data+=1
				
			if elapsed_time - function3PreviousMillis >= function3Interval:

	def smooth_value(self):
		
		#self.index = len(self.input_data)#might take much time
		self.index = self.count
		start_index = max(0, self.index-5)
		end_index = self.index
		
		clipped_data = self.input_data[start_index:end_index]
		
		mean_value = sum(clipped_data)/len(clipped_data)

		return int(mean_value)

	def set_value(self, value):
		self.input_data.append(value)
		
		self.count+=1
	
	def set_mean(self, value):
		self.mean_data.append(value)
		
		self.mean_value_count+=1
		
	def ifFull(self):
		if self.count == 200:
			self.input_data = array.array('i')
			self.count = 0
		if self.mean_value_count == 200:
			#might be better to store pointers in c++
			plt.plot(range(len(self.mean_data)), self.mean_data)
			plt.show()
			self.l_input_data.append(self.mean_data)
			self.mean_data = array.array('i')
			self.mean_value_count = 0
			
def makeplot(y):
	plt.plot(range(len(y)), y)
	
	plt.xlabel('X Range')
	plt.ylabel('Y Range')
	plt.title('Plot of X Range and Y Range')

	plt.show()

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

def approximate(data):
	inclination = array.array('i')
	differ = 10
	for index, data_ele in enumerate(data):
		data_ele = data[index]
		#approximete
		if abs(data_ele) <= differ:
			data_ele = 0
		elif data_ele > differ:
			data_ele = 1
		else:
			data_ele = -1
	print(inclination)
	return inclination

def makeplot(x, y):
	plt.plot(x, y)

	plt.xlabel('X Range')
	plt.ylabel('Y Range')
	plt.title('Plot of X Range and Y Range')

	plt.show()
	
if __name__ == '__main__':
	file_path = "C:\\Users\\kiho\\OneDrive\\デスクトップ\\morse\\KihoData.txt"
	file = open(file_path, 'r')
	morse(file).main()
	