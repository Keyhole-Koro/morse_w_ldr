import time
import array
import datetime
import threading#is not implemented in arduino

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
		
	def main(self):
		start_time = time.time()
		function1Interval = 20
		function2Interval = 1000
		
		function1PreviousMillis = 0
		function2PreviousMillis = 0
		
		while True:
			#in milliseconds which is equivalent to millis in arduino
			elapsed_time = elapsed(start_time)
			
			if elapsed_time - function1PreviousMillis >= function1Interval:
				new_value = int(self.data[self.index])#get values from ldr
				
				self.set_value(new_value)

				smoothed_value = self.smooth_value(new_value)
				
				self.set_mean(smoothed_value)
				
				print(self.mean_value_count, smoothed_value)
				self.isFull()
				
				seconds = time.time()
				function1PreviousMillis = elapsed_time

			if elapsed_time - function2PreviousMillis >= function2Interval:#calculus
				print('called2')
				function2PreviousMillis = elapsed_time


	def smooth_value(self, new_value):
		
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
		
	def isFull(self):
		if self.mean_value_count == 200:
			self.l_input_data.append(self.mean_data)
			self.mean_data = array.array('i')
			self.mean_value_count = 0
		if self.count == 200:
			self.l_input_data.append(self.input_data)
			self.input_data = array.array('i')
			self.count = 0

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
	