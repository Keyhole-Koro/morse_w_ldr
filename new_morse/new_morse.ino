#include <Arduino.h>
#include <ArduinoSTL.h>
#include <limits.h>

unsigned long previousTime1 = 0;
unsigned char interval1 = 5;

unsigned long previousTime2 = 0;
unsigned int interval2 = 50;

const int limit_input_data = 150;

char threshold = 5;

std::vector<std::vector<signed char>> list_smoothed_data;
std::vector<signed char> inclination_data;
std::vector<int> previous_data(5);

signed char noisefilter(signed char previous, signed char current);
signed char converge_to_zero(signed char initial_value, signed char convergence_rate);

std::vector<signed char> smooth_data(const std::vector<signed char>& data, signed char window_size);
std::vector<signed char> ifUndulations(const std::vector<signed char>& data, signed char threshold);

bool push_ongoing = false;

int max = 0;
int previous_input_value = 0;
int min = 0;

void dispMemory() {
  Serial.print(F("Free memory=")); 
  Serial.print(freeRam(), DEC);
  Serial.println(F("[bytes]"));
}

int freeRam() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void setup() {
  Serial.begin(9600);
  Serial.println(" ");
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - previousTime1 >= interval1) {
    previousTime1 = currentTime;
    int input_value = analogRead(A0);

    if (previous_input_value == 0) {
      previous_input_value = input_value;
    }

    int raw_inclination = (input_value - previous_input_value);

    previous_data.erase(previous_data.begin());
    previous_data.push_back(raw_inclination);

    if (previous_data.empty()){
      if (previous_input_value == 0) {
      previous_input_value = input_value;
      for (int i = 0; i < previous_data.size(); i++){
        previous_data[i] = 1;
      }
    }
    }

    int inclination_sum = 0;
    for (int data : previous_data){
      inclination_sum += data;
    }

    /*
    signed char inclination;
    if (mean_inclination >= SCHAR_MAX) {
      inclination = SCHAR_MAX;
    } else if (mean_inclination <= SCHAR_MIN) {
      inclination = SCHAR_MIN;
    } else {
      inclination = static_cast<signed char>(mean_inclination);
    }
    */
    int inclination = 0;
    if (inclination_sum!=0){
      inclination = inclination_sum/5;
    }
    
    if (inclination < 0 && inclination - min < -threshold && !push_ongoing){
      inclination_data.clear();
      Serial.print("start");
      push_ongoing = true;
    }
    if (inclination > 0 && inclination - max > threshold && push_ongoing){
      Serial.print("end");
      for (int data : inclination_data){
        Serial.print(data);
        Serial.print(" ");
      }
      push_ongoing = false;
      list_smoothed_data.push_back(inclination_data);
      inclination_data.clear();
      max = 0;
      min = 0;
    }

    if (min > inclination){
      min = inclination;
    }
    if (max < inclination){
      max = inclination;
    }

    if (push_ongoing){
      inclination_data.push_back(inclination);
    }

    previous_input_value = input_value; 
  }

  if (currentTime - previousTime2 >= interval2) {
    if (!list_smoothed_data.empty()) {
      std::vector<signed char>& input_vector = *list_smoothed_data.begin();

      std::vector<signed char> smoothed_data = smooth_data(input_vector, 5);
      //bool undulations = ifUndulations(smoothed_data, threshold);

      // Perform further operations with smoothed_data and undulations
    }
  }
}

signed char noisefilter(signed char previous, signed char current) {
    signed char difference = abs(previous - current);
    signed char k;
    
    if (difference > 3 && abs(current) < 50) {
        k = 1;
    } else {
        k = 0;
    }
    
    return converge_to_zero(current, k);
}

signed char converge_to_zero(signed char initial_value, signed char convergence_rate) {
    signed char current_value = initial_value;
    current_value -= current_value * convergence_rate;

    return current_value;
}


std::vector<signed char> smooth_data(const std::vector<signed char>& data, signed char window_size) {
  std::vector<signed char> smoothed_data;
  int data_size = data.size();

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
    smoothed_data.push_back(cumulative_sum / window_size);
  }

  return smoothed_data;
}

std::vector<signed char> ifUndulations(const std::vector<signed char>& data, signed char threshold) {
  std::vector<signed char> result;

  // Perform operations to detect undulations based on the data and threshold

  return result;
}
