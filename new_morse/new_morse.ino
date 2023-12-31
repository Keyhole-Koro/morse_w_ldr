#include <Arduino.h>
#include <ArduinoSTL.h>
#include <string>
#include <map>
#include <avr/pgmspace.h>


int threshold = 400;

unsigned long previousTime1 = 0;
unsigned char interval1 = 10;

unsigned long previousTime2 = 0;
unsigned char interval2 = 50;

unsigned long previousTime3 = 0;
unsigned char interval3 = 500;

std::vector<signed char> data;
std::string morse_input;
std::vector<std::string*> list_morse_input;

unsigned char on_count = 0;
signed char off_count = 0;

bool if_on = false;

String decodedMessage;

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
  dispMemory();
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - previousTime1 >= interval1) {
    previousTime1 = currentTime;
    int input_value = analogRead(A0);
    if (input_value < threshold) {
      if_on = true;
    } else {
      if_on = false;
    }

    if (if_on && on_count < 127) {
      on_count++;
    } else if (off_count > -128) {
      off_count--;
    }

    if (!if_on && on_count > 0) {
      data.push_back(off_count);
      data.push_back(on_count);
      on_count = 0;
      off_count = 0;
    }
  }

  if (currentTime - previousTime2 >= interval2) {
    previousTime2 = currentTime;
    if (!list_morse_input.empty()) {
      std::string morse;
      bool period = false;
      bool no_match = false;
      for (std::string* morse : list_morse_input){
        Serial.print("morse->c_str()");
        Serial.print(morse->c_str());
          for (int i = 0; i < 26; i++) {
            if (morse == morseCode[i]) {
                decodedMessage += (char)('A' + i);  // Convert index to corresponding letter
                //no_match = true;
                break;
            }
        }
      }
      //Serial.print(decodedMessage);
      list_morse_input.clear();
    }
      
  }

  if (currentTime - previousTime3 >= interval3) {
    previousTime3 = currentTime;
    if (!data.empty()) {
      unsigned char index;
      for (index = 0; index < data.size(); index++) {    
        if (data[index] > 0){
          morse_input += (data[index] < 20) ? "." : "-";
        } else if (data[index] < -50) {
          list_morse_input.push_back(&morse_input);
          morse_input.clear();
        }
      }
      if (!morse_input.empty()) {
        //Serial.print(morse_input.c_str());
      }
      data.clear();
    }
  }
}

