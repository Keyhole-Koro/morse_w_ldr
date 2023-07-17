#include <Arduino.h>
#include <ArduinoSTL.h>
#include <string>


int threshold = 300;

unsigned long previousTime1 = 0;
unsigned char interval1 = 10;

unsigned long previousTime2 = 0;
unsigned char interval2 = 50;

unsigned long previousTime3 = 0;
unsigned char interval3 = 50;

std::vector<signed char> data;
std::vector<std::string> morse_input;

unsigned char on_count = 0;
signed char off_count = 0;

bool if_on = false;

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
  Serial.println(" ");
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - previousTime1 >= interval1) {
    previousTime1 = currentTime;
    int input_value = analogRead(A0);
    if (input_value < threshold){
      if_on = true;
    }else{
      if_on = false;
    }

    if (if_on && on_count < 127){
      on_count++;
    }else if (off_count > -128){
      off_count--;
    }

    if (!if_on && on_count > 0){
      data.push_back(off_count);
      data.push_back(on_count);
      on_count = 0;
      off_count = 0;
    }

    
  }
  if (currentTime - previousTime2 >= interval2) {
    previousTime2 = currentTime;
    if (!morse_input.empty()){
      int index = 0;
      String decodedMessage;
      String morse;
      bool period = false;
      bool no_match = false;
      for (std::string value : morse_input){
          Serial.print(value);
          if (value = "_"){
            period = true;
          }
          if (no_match) {
            Serial.print("?");
          }
          morse += value;
          if (period){
            for (int j = 0; j < 26; j++) {
              if (morse == morseCode[j]) {
                  decodedMessage += (char)('A' + j);
                  Serial.print(decodedMessage);
                  index++;
                  morse_input.erase(morse_input.begin(), morse_input.begin()+index);//takes so much time
                  break;
              }
            }
            period = false;
          }
        index++;
      }
    }
  }
  
  if (currentTime - previousTime3 >= interval3) {
      previousTime3 = currentTime;
      if (!data.empty()) {
          for (int i = 0; i < data.size(); i++) {
              if (data[i + 1] < -50) {
                  morse_input.push_back((data[i] < 10) ? "." : "-");
              } else if (data[i] > 0) {
                  morse_input.push_back("_");//blank
              }
          }
          data.clear();
      }
  }
}