#include <Arduino.h>
#include <ArduinoSTL.h>
#include <limits.h>

unsigned long previousTime1 = 0;
unsigned char interval1 = 3;

unsigned long previousTime2 = 0;
unsigned int interval2 = 60;

unsigned long previousTime3 = 0;
unsigned int interval3 = 10;


const char ele_sum = 10;
const char interval_sum = 3;

bool first_signal = true;
bool upward_found = true;
signed char on_count = 0;
signed char off_count = 0;

int previous_value = 0;

signed char threshold = 40;

std::string morse_input;

std::string morse;
std::vector<signed char> input_data;
std::vector<std::string> input_morse;

int former_sum = 0;
int later_sum = 0;
int temp_sum_index = 0;

char decodedMessage;

const char* morseCode[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--",
                            "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."};

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
  previous_value = analogRead(A0);
  dispMemory();
  }

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - previousTime1 >= interval1) {

    previousTime1 = currentTime;
    int new_value = analogRead(A0);
    int inclination = (new_value - previous_value)/2;

    if (temp_sum_index == 10){
      int int_mean = (former_sum+later_sum);///temp_sum_index
      signed char char_sum = convert_int_char(int_mean);
      input_data.push_back(char_sum);
      former_sum = later_sum;
      later_sum = 0;
      temp_sum_index = 5;
    }

    if (temp_sum_index < 5){
      former_sum += inclination;
    } else {
      later_sum += inclination;
    }

    temp_sum_index++;
    previous_value = new_value;
  }

  if (currentTime - previousTime2 >= interval2) {
    previousTime2 = currentTime;
    if (!input_morse.empty()) {
      for (int i = 0; i < input_morse.size(); i++){
        std::string morse_input = std::move(input_morse.front());
        decodedMessage = "_";
        for (int i = 0; i < 26; i++) {
          if (morse_input == morseCode[i]) {
            decodedMessage = static_cast<char>('A' + i);
            break;
          }
        }
        
      }
      input_morse.clear();
      Serial.print(decodedMessage);
    }
  }

  if (currentTime - previousTime3 >= interval3) {
    previousTime3 = currentTime;
    if (!input_data.empty()){
      for (signed char i = 0; i < input_data.size(); i++){
        if (first_signal && input_data[i] < -threshold){
          on_count = 0;
          upward_found = false;
          first_signal = false;
        }

        if (upward_found && input_data[i] < -threshold){
          on_count = 0;
          upward_found = false;
        } else if (!upward_found && input_data[i] > threshold) {
            upward_found = true;
            off_count = 0;
            morse += (on_count < 10) ? "." : "-";
          }
        if (on_count < 127){
          on_count++;
        }
        if (off_count > -128)
          off_count--;

        if(!morse.empty() && upward_found && off_count < -30) {
          input_morse.push_back(std::move(morse));
          morse.clear();
          off_count = 0;
        }
        }
      input_data.clear(); 
    }
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