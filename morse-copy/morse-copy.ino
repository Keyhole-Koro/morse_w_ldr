#include <Arduino.h>
#include <ArduinoSTL.h>
#include <limits.h>

unsigned long previousTime1 = 0;
unsigned char interval1 = 5;

unsigned long previousTime2 = 0;
unsigned int interval2 = 50;

unsigned long previousTime3 = 0;
unsigned int interval3 = 50;

signed char empty_ = -99;

const int limit_input_data = 100;

std::vector<std::vector<signed char>> list_smoothed_data;
std::vector<signed char> input_data(limit_input_data);

std::vector<signed char> temporary_previous_data;
std::vector<signed char> plus_minus_length;

int input_index = 0;
int previous_value = 0;

std::vector<signed char> smooth_data(const std::vector<signed char>& data, signed char window_size, int data_size);
std::vector<signed char> extract_zero(const std::vector<signed char>& values, signed char threshold, signed char steps);
std::vector<signed char> ifUndulations(const std::vector<signed char>& values, const std::vector<signed char>& zero_indices, signed char threshold);

void dispMemory(){
  Serial.print(F("Free memory=")); 
  Serial.print(freeRam(), DEC);
  Serial.println(F("[bytes]"));
}

int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void setup() {
  Serial.begin(9600);
  }

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - previousTime1 >= interval1) {
    //Serial.print(currentTime - previousTime1);
    //Serial.println(" ");
    
    previousTime1 = currentTime;
    int new_value = analogRead(A0);
    
    if (previous_value == 0) {
      previous_value = new_value;
    }

    if (input_index == limit_input_data) {
      std::vector<signed char> smoothed_data = smooth_data(input_data, 5, limit_input_data);
      list_smoothed_data.push_back(std::move(smoothed_data));
      /*
      for (int i = 0; i < smoothed_data.size(); i++) {
          Serial.print(smoothed_data[i]);
          Serial.print(" ");
        }
      */
      input_index = 0;
    }
    int raw_inclination = (new_value - previous_value);

    signed char inclination;
    if (raw_inclination > SCHAR_MAX) {
      inclination = SCHAR_MAX;
    } else if (raw_inclination < SCHAR_MIN) {
      inclination = SCHAR_MIN;
    } else {
      inclination = static_cast<signed char>(raw_inclination);
    }

    input_data[input_index] = inclination;
    input_index++;

    previous_value = new_value;
  }

  if (currentTime - previousTime2 >= interval2) {
    previousTime2 = currentTime;
    if (!list_smoothed_data.empty()) {
      std::vector<signed char>& begin_vector = *list_smoothed_data.begin();
      std::vector<signed char> combined_vector;
      signed char threshold = 10;
      signed char steps = 3;
      //i hate this method
	    combined_vector.insert(combined_vector.end(), temporary_previous_data.begin(), temporary_previous_data.end());
      combined_vector.insert(combined_vector.end(), begin_vector.begin(), begin_vector.end());

	    temporary_previous_data.clear();

      std::vector<signed char> zero_indices = extract_zero(combined_vector, threshold, steps);
      if (!zero_indices.empty()){

        std::vector<signed char> undulations = ifUndulations(combined_vector, zero_indices, 40);//temporary

        for (int i = 0; i < undulations.size(); i++){
          plus_minus_length.push_back(undulations[i]);
          //Serial.print(undulations[i]);
          //Serial.print(" ");
        }

        int zero_indices_back = zero_indices.back();

        if (zero_indices_back>0){//store as previous_data
          for(int i = zero_indices_back; i < combined_vector.size(); i++){
            temporary_previous_data.push_back(zero_indices[i]);
          }
        }
        
        combined_vector.clear();
        zero_indices.clear();
        undulations.clear();
      }else{
        plus_minus_length.push_back(empty_);
      }

      list_smoothed_data.erase(list_smoothed_data.begin());
    }
  }
  
  if (currentTime - previousTime3 >= interval3){
    //determine_morse();
    if (!plus_minus_length.empty()){
      plus_minus_length.erase(plus_minus_length.begin());
    }
  }
}

std::vector<signed char> smooth_data(const std::vector<signed char>& data, signed char window_size, int data_size) {
  std::vector<signed char> smoothed_data(data_size, 0);
  
  int half_window = window_size / 2;
  
  int cumulative_sum = 0;
  for (int i = 0; i < window_size; i++) {
    cumulative_sum += data[i];
  }
  
  for (int i = 0; i < data_size; i++) {
    if (i > half_window) {
      cumulative_sum -= data[i - half_window - 1];
    }
    if (i + half_window < data_size) {
      cumulative_sum += data[i + half_window];
    }
    smoothed_data[i] = cumulative_sum / window_size;
  }
  
  return std::move(smoothed_data);
}


std::vector<signed char> extract_zero(const std::vector<signed char>& data, signed char threshold, signed char steps) {
  std::vector<signed char> indices;
  int num_index = data.size();
  int start_index = 0;
  int count = 0;
  int value = 0;
  while (start_index < num_index) {
    value = data[start_index];
    if (abs(value) <= threshold) {
      count = 1;
      while (start_index + count < num_index) {
        int next_value = data[start_index + count];

        if (abs(next_value) > threshold) {
            break;
        }
        count++;
      } 
      if (count >= steps - 1 && count != num_index) {
        int zero_start_index =  start_index;
        int zero_end_index =  start_index + count -1;
        indices.push_back(zero_start_index);
        indices.push_back(zero_end_index);
      }
    }
    start_index += count;
    start_index += steps;
  }
  return std::move(indices);//in previous data be gonna - current data be gonna +
}


std::vector<signed char> ifUndulations(const std::vector<signed char>& data, const std::vector<signed char>& zero_indices, signed char threshold) {
  std::vector<signed char> undulations;

  unsigned char data_size = data.size();

  for (int i = 0; i < zero_indices.size(); i += 2) {
    unsigned start_index = zero_indices[i];
    unsigned end_index = zero_indices[i + 1];

    signed char start = data[start_index];
    signed char end = data[end_index];

    int sum_previous = 0;//should be minus
    int sum_next = 0;

    unsigned char length = end_index - start_index + 1;
	  
	  for (signed char previousIndex = start_index; previousIndex > 0; previousIndex--){
      signed char previous_value = data[previousIndex];
      if (previous_value > -3){
        break;
      }
      sum_previous += previous_value;
    }

    for (signed char nextIndex = end_index; nextIndex < data_size; nextIndex++){
      signed char next_value = data[nextIndex];
      if (next_value < 3){
        break;
      }
      sum_next += next_value;
    }

    if (sum_previous < -threshold && sum_next > threshold) {
      undulations.push_back(length);
    } else {
      undulations.push_back(-length);
    }
  }

  return std::move(undulations);
}



/*
void determine_morse(){
  if (!plus_minus_length.empty()){

  }
}*/
