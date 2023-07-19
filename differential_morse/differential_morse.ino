#include <Arduino.h>
#include <ArduinoSTL.h>
#include <limits.h>

unsigned long previousTime1 = 0;
unsigned char interval1 = 2;

unsigned long previousTime2 = 0;
unsigned int interval2 = 50;

unsigned long previousTime3 = 0;
unsigned int interval3 = 50;

const int limit_input_data = 100;
const char ele_sum = 10;

int previous_value = 0;

std::vector<signed char> input_data(limit_input_data);

int temp_sum = 0;
int temp_sum_index = 0;

void setup() {
  Serial.begin(9600);
  previous_value = analogRead(A0);
  }

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - previousTime1 >= interval1) {
    previousTime1 = currentTime;
    int new_value = analogRead(A0);

    int inclination = (new_value - previous_value)/2;

    if (temp_sum_index == 5){
      int int_mean = temp_sum/temp_sum_index;
      signed char char_sum = convert_int_char(int_mean);
      input_data.push_back(char_sum);
      Serial.print(input_data.back());
      Serial.print(" ");
      temp_sum = 0;
      temp_sum_index = 0;
    }
    temp_sum += inclination;
    temp_sum_index++;
    previous_value = new_value;
  }
  if (currentTime - previousTime2 >= interval2) {
    previousTime2 = currentTime;
    input_data.clear();

  }
}

signed char convert_int_char(int int_input) {
  signed char output;
  if (int_input > SCHAR_MAX) {
    output = SCHAR_MAX;
  } else if (int_input < SCHAR_MIN) {
    output = SCHAR_MIN;
  } else {
    output = static_cast<signed char>(int_input);
  }
  return output;
}