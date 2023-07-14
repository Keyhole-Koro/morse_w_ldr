#include <iostream>
#include <fstream>
#include <vector>
#include <array>

#define UP 1;
#define FLAT 0;
#define DOWN -1;

unsigned long previousTime1 = 0;
unsigned long interval1 = 20;

unsigned long previousTime2 = 0;
unsigned long interval2 = 100;

/*value is single
	data is multiple like list array
	(in most cases)*/

const int limit_input_data = 50;

std::vector<std::vector<int>> l_input_data;
array<int,limit_input_data> input_data;//dont have to remove all elements

std::vector<int> store_previous_data;
std::vector<std::vector<int>> plus_minus_length;

int input_index = 0;
int previous_value = 0;

void setup() {
  
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - previousTime1 >= interval1) {
    previousTime1 = currentTime;
    int new_value = analogRead(A0);
    
    if (previous_value == 0){
      previous_value = new_value
    }

    if (input_index == limit_input_data){
      std::vector<int> smoothed_data = smooth_data(input_data, 5, limit_input_data);
      l_input_data.push_back(smoothed_data);
      input_index = 0;
    }
    inclination = (new_value - previous_value)/2;

    input_data[input_index] = inclination;
    input_index++;

    previous_value = new_value;
  }

  if (currentTime - previousTime2 >= interval2) {
    previousTime2 = currentTime;
    if (!l_input_data.empty()) {
      std::vector<int>& begin_vector = l_input_data.front();
      int threshold = 3;
      int steps = 3;
      std::vector<int> zero_indices = extract_zero(begin_vector, threshold, steps);

      std::vector<int> undulations = ifUndulations(begin_vector, zero_indices, threshold);
      plus_minus_length.push_back(undulations);
      
       int lastElement_index = zero_indices.back();
      begin_vector.erase(begin_vector.begin(), begin_vector.begin() + lastElement_index);
      store_previous_data = begin_vector;
      //int* memory_location = l_input_data.data();
      //dont forget to delete no need vectors
      l_input_data.erase(l_input_data.begin());
    }
  }
  if (currentTime - previousTime3 >= interval3){
    determine_morse();
  }

}

std::vector<int> smooth_data(const std::vector<int>& data, int window_size, int data_size) {
		std::vector<int> smoothed_data(data_size, 0);
		std::vector<int> cumulative_sum(data_size, 0);

		cumulative_sum[0] = data[0];
		for (int i = 1; i < data_size; i++) {
			cumulative_sum[i] = cumulative_sum[i - 1] + data[i];
		}

		int half_window = window_size / 2;
		for (int i = 0; i < data_size; i++) {
			int start_index = std::max(0, i - half_window);
			int end_index = std::min(static_cast<int>(data_size) - 1, i + half_window);
			int window_size_actual = end_index - start_index + 1;
			smoothed_data[i] = (cumulative_sum[end_index] - cumulative_sum[start_index] + data[start_index]) / window_size_actual;
		}

		return smoothed_data;
	}

std::vector<int> extract_zero(const std::vector<int>& values, int threshold) {
    std::vector<int> indices;
    int steps = 3;
    int start_index = 0;

    while (start_index < values.size()) {
        if (std::abs(values[start_index]) <= threshold) {
            int count = 1;
            while (start_index + count < values.size() && std::abs(values[start_index + count]) <= threshold) {
                count++;
            }

            if (count >= steps - 1) {
                indices.push_back(start_index);
                indices.push_back(start_index + count - 1);
            }
        }

        start_index += steps;
    }

    return indices;
}

std::vector<int> ifUndulations(const std::vector<int>& indices, const std::vector<int>& zero_indices, int threshold) {
    std::vector<int> undulations;

    for (int i = zero_indices[0]; i < zero_indices.size(); i += 2) {
        int start_index = i;
        int end_index = i + 1;
        int start = indices[start_index];
        int end = indices[end_index];

        int previous = indices[start_index-1];
        int next = indices[end_index+1]

        if (std::abs(start_index - start) > threshold && std::abs(end - next) > threshold) {//modify
          undulations.push_back(end-start);
        }else{
          undulations.push_back(-(end-start));
        }
    }

    return undulations;
}

void determine_morse(){
  if (!plus_minus_length.empty()){

  }
}