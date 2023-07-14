#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <chrono>

class Morse {
public:
	Morse(std::ifstream& inputFile) : file(std::move(inputFile)), count(0), index(0) {
		int value;
		while (file >> value) {
			data.push_back(value);
		}
	}

	void main() {
		auto start_time = std::chrono::steady_clock::now();
		int function1Interval = 20;
		int function2Interval = 100;
		int function3Interval = 10000;

		std::chrono::steady_clock::time_point function1PreviousMillis = start_time;
		std::chrono::steady_clock::time_point function2PreviousMillis = start_time;
		std::chrono::steady_clock::time_point function3PreviousMillis = start_timeunsigned long previousTime1 = 0;
unsigned long interval1 = 1000;

unsigned long previousTime2 = 0;
unsigned long interval2 = 500;

void setup() {
  // Initialize your setup code here
}

void loop() {
  unsigned long currentTime = millis();

  // Function 1: Executes every 1 second
  if (currentTime - previousTime1 >= interval1) {
    previousTime1 = currentTime;
    function1(); // Call your first function
  }

  // Function 2: Executes every 0.5 seconds
  if (currentTime - previousTime2 >= interval2) {
    previousTime2 = currentTime;
    function2(); // Call your second function
  }

  // Other non-blocking tasks can be performed here

  // Function 3: Executes continuously
  function3(); // Call your third function

  // Other non-blocking tasks can be performed here

  // Add more functions as needed

  // Rest of your code
}

void function1() {
  // Your code for function 1
}

void function2() {
  // Your code for function 2
}

void function3() {
  // Your code for function 3
}
;

		int previous_value = 0;
		int data_index = 0;

		std::vector<int> all_data;

		while (true) {
			std::chrono::steady_clock::time_point elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time);

			if (elapsed_time - function1PreviousMillis >= function1Interval) {
				function1PreviousMillis = std::chrono::steady_clock::now();


				int new_input_value = data[data_index];

				if (previous_value == 0) {
					previous_value = new_input_value;
				}

				int inclination = (new_input_value - previous_value) / 2;

				previous_value = new_input_value;

				if (index == 50) {
					std::vector<int> smoothed_data = smooth_data(input_data, 5);
					all_data.insert(all_data.end(), smoothed_data.begin(), smoothed_data.end());
					input_data.clear();
					index = 0;
				}

				input_data.push_back(std::chrono::round(inclination));
				index++;
				data_index++;
			}

			if (elapsed_time - function2PreviousMillis >= function2Interval) {
				function2PreviousMillis = std::chrono::steady_clock::now();

				if (!l_input_data.empty()) {
					all_data.insert(all_data.end(), l_input_data.front().begin(), l_input_data.front().end());
					l_input_data.erase(l_input_data.begin());
				}
			}

			if (elapsed_time - function3PreviousMillis >= function3Interval) {
				// Call your plotting function here
				// plot(all_data);

				function3PreviousMillis = std::chrono::steady_clock::now();
			}
		}
	}

private:
	std::ifstream file;
	std::vector<int> data;
	std::vector<std::vector<int>> l_input_data;
	int count;
	int index;
	std::vector<int> input_data;

	std::vector<int> smooth_data(const std::vector<int>& data, int window_size) {
		std::vector<int> smoothed_data(data.size(), 0);
		std::vector<int> cumulative_sum(data.size(), 0);

		cumulative_sum[0] = data[0];
		for (int i = 1; i < data.size(); i++) {
			cumulative_sum[i] = cumulative_sum[i - 1] + data[i];
		}

		int half_window = window_size / 2;
		for (int i = 0; i < data.size(); i++) {
			int start_index = std::max(0, i - half_window);
			int end_index = std::min(static_cast<int>(data.size()) - 1, i + half_window);
			int window_size_actual = end_index - start_index + 1;
			smoothed_data[i] = (cumulative_sum[end_index] - cumulative_sum[start_index] + data[start_index]) / window_size_actual;
		}

		return smoothed_data;
	}
};

int main() {
	std::ifstream inputFile("path/to/your/input/file.txt");
	Morse morse(inputFile);
	morse.main();

	return 0;
}

