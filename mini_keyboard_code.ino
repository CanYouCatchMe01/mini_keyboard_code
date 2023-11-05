// NOTE: This sketch file is for use with Arduino Leonardo and
//       Arduino Micro only.
//
// Joystic librery by Matthew Heironimus
// 2016-11-24
//
// Code by Marcus Cazzola
// 2020-02-05
//--------------------------------------------------------------------
#include <Keyboard.h>

#define SHIFTING_PINS_SIZE 4
#define INPUT_PINS_SIZE 5

byte shifting_pins[SHIFTING_PINS_SIZE] = {A6, A7, A8, A9};
byte input_pins[INPUT_PINS_SIZE] = {10,16,14,15,18};
byte joystick_button_pin = 19;
byte VRX = A2, VRY = A3;

//main keys
char keys[SHIFTING_PINS_SIZE * INPUT_PINS_SIZE] = {
  KEY_ESC, '2', '3', '4', '5',
  'q', 'w', 'e', 'r', 't',
  'a','s', 'd', 'f', 'g',
  'z', 'x', 'c', 'v', 'b'};

//special keys
char joystick_btn = 'm', forward = '0', back = '8', right = '7', left = '6';

void setup() {
  Serial.begin(9600);
  Keyboard.begin();
  
  //input_pins
  for (byte i = 0; i < INPUT_PINS_SIZE; i++)
  {
    pinMode(input_pins[i], INPUT_PULLUP);
  }

  //shifting_pins
  for (byte i = 0; i < INPUT_PINS_SIZE; i++)
  {
    pinMode(shifting_pins[i], OUTPUT);
    digitalWrite(shifting_pins[i], HIGH);
  }

  //joystick
  pinMode(joystick_button_pin, INPUT_PULLUP);
  pinMode(VRX, INPUT_PULLUP);
  pinMode(VRY, INPUT_PULLUP);
}

void read_main_buttons()
{
  for(byte s = 0; s < SHIFTING_PINS_SIZE; s++) //För varje input pin
  {
    digitalWrite(shifting_pins[s], LOW);
    for(byte i = 0; i < INPUT_PINS_SIZE; i++) //Loop trought shifting_pins
    {
      byte loop_times = s*i+i;
      bool now_pressed = !digitalRead(input_pins[i]);
      now_pressed ? Keyboard.press(keys[loop_times]) : Keyboard.release(keys[loop_times]);
    }
    digitalWrite(shifting_pins[s], HIGH);
  }
}

void read_joystick_keys(int dead_zone = 100, int mid_x = 590, int mid_y = 570)
{
  //back
  if(analogRead(VRY) > mid_y + dead_zone)
  {
    Keyboard.press(back);
  }
        
  else
  {
    Keyboard.release(back);
  }

  //forward
  if(analogRead(VRY) < mid_y - dead_zone)
  {
    Keyboard.press(forward);
  }
        
  else
  {
    Keyboard.release(forward);
  }
  
  //right
  if(analogRead(VRX) > mid_x + dead_zone)
  {
    Keyboard.press(right);
  }
        
  else
  {
    Keyboard.release(right);
  }

  //left
  if(analogRead(VRX) < mid_x - dead_zone)
  {
    Keyboard.press(left);
  }
        
  else
  {
    Keyboard.release(left);
  }
}


void loop() {
  read_main_buttons();
  Serial.println(!digitalRead(joystick_button_pin));
  Serial.println(analogRead(VRX));
  Serial.println(analogRead(VRY));
  read_joystick_keys();
  delay(1);
}
