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
#include <Joystick.h>
#include <Mouse.h>

byte shifting_pins[] = {A6, A7, A8, A9};
byte input_pins[] = {10,16,14,15,18};
byte joystick_button_pin = 19;
byte VRX = A2, VRY = A3;

byte len_shifting_pins = sizeof(shifting_pins)/sizeof(shifting_pins[0]); //sizeof() = memory size
byte len_input_pins = sizeof(input_pins)/sizeof(input_pins[0]); //sizeof() = memory size

//main keys
char keys[] = {
  KEY_ESC, '2', '3', '4', '5',
  'q', 'w', 'e', 'r', 't',
  'a','s', 'd', 'f', 'g',
  'z', 'x', 'c', 'v', 'b'};

//special keys
char joystick_btn = 'm', forward = '9', back = '8', right = '7', left = '6';

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  1, 0,                  // Button Count, Hat Switch Count
  true, true, false,     // X, Y, Z Axis
  false, false, false,   // Rx, Ry, Rz (Rotation)
  false, false,          // rudder, throttle
  false, false, false);  // accelerator, brake, steering

void setup() {
  Serial.begin(9600);
  Joystick.begin();
  Keyboard.begin();
  Mouse.begin();
  
  
  //input_pins
  for (byte i = 0; i < len_input_pins; i++)
  {
    pinMode(input_pins[i], INPUT_PULLUP);
  }

  //shifting_pins
  for (byte i = 0; i < len_input_pins; i++)
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
  byte loop_times = 0;
  for(byte s = 0; s < len_shifting_pins; s++) //För varje input pin
  {
    digitalWrite(shifting_pins[s], LOW);
    for(byte i = 0; i < len_input_pins; i++) //Loop trought shifting_pins
    {
      //Press
      if(!digitalRead(input_pins[i]))
      {
        if (loop_times == 11)
        {
          Mouse.press(MOUSE_RIGHT);
        }
        else if (loop_times == 12)
        {
          Mouse.press(MOUSE_LEFT);
        }
        else
        {
          Keyboard.press(keys[loop_times]);
        }
      }
      //Release
      else
      {
        if (loop_times == 11)
        {
          Mouse.release(MOUSE_RIGHT);
        }
        else if (loop_times == 12)
        {
          Mouse.release(MOUSE_LEFT);
        }
        else
        {
          Keyboard.release(keys[loop_times]);
        }
      }
      loop_times++;
    }
    digitalWrite(shifting_pins[s], HIGH);
  }
}

void read_joystick_button()
{
  if(!digitalRead(joystick_button_pin))
  {
    Joystick.setButton(0, 1);
    Keyboard.press(joystick_btn);
  }
  else
  {
    Joystick.setButton(0, 0);
    Keyboard.release(joystick_btn);
  }
}

void read_joystick_controller()
{
  Joystick.setXAxis(analogRead(VRX));
  Joystick.setYAxis(analogRead(VRY));
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
  read_joystick_button();
  read_joystick_controller();
  //read_joystick_keys();
  delay(1);
}
