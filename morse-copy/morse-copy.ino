#include <Arduino.h>
#include <ArduinoSTL.h>

unsigned long previousTime1 = 0;
unsigned long interval1 = 20;

unsigned long previousTime2 = 0;
unsigned long interval2 = 100;

unsigned long previousTime3 = 0;
unsigned long interval3 = 100;

const int limit_input_data = 100;

std::vector<int> smoothed_data(limit_input_data);
std::vector<int> input_data(limit_input_data);

std::vector<int> temporary_last_data;
std::vector<std::vector<int>> plus_minus_length;

int input_index = 0;
int previous_value = 0;

std::vector<int> smooth_data(const std::vector<int>& data, int window_size, int data_size);
std::vector<int> extract_zero(const std::vector<int>& values, int threshold, int steps);
std::vector<int> ifUndulations(const std::vector<int>& values, const std::vector<int>& zero_indices, int threshold);

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
      smoothed_data = smooth_data(input_data, 5, limit_input_data);
      dispMemory();
      input_index = 0;
    }
    int inclination = (new_value - previous_value)/2;

    input_data[input_index] = inclination;
    input_index++;

    previous_value = new_value;
  }

  if (currentTime - previousTime2 >= interval2) {
    previousTime2 = currentTime;
    if (!smoothed_data.empty()) {
      std::vector<int>& begin_vector = smoothed_data;
      int threshold = 5;
      int steps = 3;

      std::vector<int> zero_indices = extract_zero(begin_vector, threshold, steps);
      for (int i = 0; i < zero_indices.size(); i++) {
        Serial.print(zero_indices[i]);
        Serial.print(" ");
      }
      Serial.println(" ");
      std::vector<int> undulations = ifUndulations(begin_vector, zero_indices, threshold);

      for (int i = 0; i < undulations.size(); i++) {
        Serial.print(undulations[i]);
        Serial.println(" ");
      }

      plus_minus_length.push_back(undulations);

      temporary_last_data.clear();
      /*
      int zero_indices_back = zero_indices.back();
      if (zero_indices.size() != 2 || zero_indices_back != 99){//in case empty have [0 99]
        int last_indice = zero_indices[zero_indices.size() - 1];//-1 for emitting 99
        for(int i = last_indice; i > (limit_input_data-1); i++){
          temporary_last_data.push_back(zero_indices[i]);
        }
        Serial.print("last_indice=");
        Serial.print(last_indice);
      }
      Serial.print("temporary_size=");
      Serial.print(temporary_last_data.size());

      */
      
      Serial.println(" ");
      
      begin_vector.clear();
      zero_indices.clear();
      undulations.clear();
    }
  }
  
  if (currentTime - previousTime3 >= interval3){
    //determine_morse();
    if (!plus_minus_length.empty()){
      plus_minus_length.erase(plus_minus_length.begin());
    }
  }
}

std::vector<int> smooth_data(const std::vector<int>& data, int window_size, int data_size) {
  std::vector<int> smoothed_data(data_size, 0);
  
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


std::vector<int> extract_zero(const std::vector<int>& values, int threshold, int steps) {
  std::vector<int> indices;
  int current_index = 0;
  int count = 0;
  while (current_index < values.size()) {
    if (abs(values[current_index]) <= threshold) {
      count = 1;
      while (current_index + count < values.size() && abs(values[current_index + count]) <= threshold) {
        count++;
      }

      if (count >= steps - 1) {
        indices.push_back(current_index);
        indices.push_back(current_index + count - 1);
      }
    }
    current_index += count;
    current_index += steps;
  }
  return std::move(indices);
}


std::vector<int> ifUndulations(const std::vector<int>& values, const std::vector<int>& zero_indices, int threshold) {
  std::vector<int> undulations;

  for (int i = zero_indices[0]; i < zero_indices.size(); i += 2) {
    int start_index = i;
    int end_index = i + 1;
    int start = values[start_index];
    int end = values[end_index];

    int previous = (start_index > 0) ? values[start_index - 1] : start;
    int next = (end_index < values.size() - 1) ? values[end_index + 1] : end;

    int indice_start = zero_indices[start_index];
    int indice_end = zero_indices[end_index];

    if (abs(previous - start) > threshold && abs(end - next) > threshold) {//modify
      undulations.push_back(indice_end-indice_start);
    }else{
      undulations.push_back(-(indice_end-indice_start));
    }
  }

    return std::move(undulations);
  }

/*
void determine_morse(){
  if (!plus_minus_length.empty()){

  }
}*/
