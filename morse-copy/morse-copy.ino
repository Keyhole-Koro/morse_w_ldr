#include <Arduino.h>
#include <ArduinoSTL.h>
#include <limits.h>

unsigned long previousTime1 = 0;
unsigned char interval1 = 20;

unsigned long previousTime2 = 0;
unsigned int interval2 = 100;

unsigned long previousTime3 = 0;
unsigned int interval3 = 1000;

signed char empty_ = -99;

const int limit_input_data = 100;

std::vector<std::vector<signed char>> list_smoothed_data;
std::vector<signed char> input_data(limit_input_data);

std::vector<signed char> temporary_previous_data;
std::vector<signed char> plus_minus_length;

int input_index = 0;
int previous_value = 0;

std::vector<signed char> smooth_data(const std::vector<signed char>& data, signed char window_size, int data_size);
std::vector<signed char> extract_zero(const std::vector<signed char>& values, const std::vector<signed char>& previous_data, signed char threshold, signed char steps);
std::vector<signed char> ifUndulations(const std::vector<signed char>& values, const std::vector<signed char>& temporary_previous_data, const std::vector<signed char>& zero_indices, signed char threshold);

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
      dispMemory();
      for (int i = 0; i < smoothed_data.size(); i++) {
          Serial.print(smoothed_data[i]);
          Serial.print(" ");
        }
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
      signed char threshold = 5;
      signed char steps = 3;

      std::vector<signed char> zero_indices = extract_zero(begin_vector, threshold, steps);
      if (!zero_indices.empty()){
        for (int i = 0; i < zero_indices.size(); i++) {
          Serial.print(zero_indices[i]);
          Serial.print(" ");
        }
        Serial.println(" ");
        std::vector<signed char> undulations = ifUndulations(begin_vector, temporary_previous_data, zero_indices, threshold);

        for (int i = 0; i < undulations.size(); i++) {
          Serial.print(undulations[i]);
          Serial.println(" ");
        }
        for (int i = 0; i < undulations.size(); i++){
          plus_minus_length.push_back(undulations[i]);
        }
      
        int zero_indices_back = zero_indices.back();
        for(int i = zero_indices_back; i < limit_input_data; i++){
          temporary_previous_data.push_back(zero_indices[i]);
        }
        Serial.print("temporary_size=");
        Serial.print(temporary_previous_data.size());
        
        Serial.println(" ");
        
        begin_vector.clear();
        zero_indices.clear();
        undulations.clear();
      }else{
        plus_minus_length.push_back(empty_);
      }

      list_smoothed_data.erase(list_smoothed_data.begin());
      temporary_previous_data.clear();
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


std::vector<signed char> extract_zero(const std::vector<signed char>& values, const std::vector<signed char>& previous_data, signed char threshold, signed char steps) {
  std::vector<signed char> indices;
  int num_new_index = values.size();
  int num_previous_index = previous_data.size();
  int whole_index = num_new_index+num_previous_index;
  int current_index = 0;
  int count = 0;
  int value = 0;
  while (current_index < whole_index) {
    value = (current_index < num_previous_index) ? previous_data[current_index]:values[current_index];
    ////////////////////
    if (abs(value) <= threshold) {
      count = 1;
      while (current_index + count < values.size() && abs(values[current_index + count]) <= threshold) {
        count++;
      }
      Serial.println(count);
      if (count >= steps - 1 && count != limit_input_data) {
        indices.push_back(current_index);
        indices.push_back(current_index + count - 1);
      }
    }
    current_index += count;
    current_index += steps;
  }
  return std::move(indices);
}


std::vector<signed char> ifUndulations(const std::vector<signed char>& values, const std::vector<signed char>& previous_data, const std::vector<signed char>& zero_indices, signed char threshold) {
  std::vector<signed char> undulations;

  for (int i = 0; i < zero_indices.size(); i += 2) {
    int start_index = zero_indices[i];
    int end_index = zero_indices[i + 1];

    signed char start = values[start_index];
    signed char end = values[end_index];

    signed char previous = (start_index > 0) ? values[start_index - 1] : previous_data.back();
    signed char next = (end_index < values.size() - 1) ? values[end_index + 1] : end;

    signed char length = end_index - start_index + 1;

    if (abs(previous - start) > threshold && abs(end - next) > threshold) {
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
