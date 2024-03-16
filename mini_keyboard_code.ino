// NOTE: This sketch file is for use with Arduino Leonardo and
//       Arduino Micro only.
//
// Joystic library by Matthew Heironimus
// https://github.com/MHeironimus/ArduinoJoystickLibrary
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

#define FORTNITE 0
#define HALO 1
#define THE_FINALS 2

#define CURRENT_GAME THE_FINALS

#if CURRENT_GAME == FORTNITE
//main keys
char keys[SHIFTING_PINS_SIZE * INPUT_PINS_SIZE] = {
  KEY_ESC, '2', '3', 'm', 'i',
  '1', '2', '3', '4', '5',
  ' ', 'x', 'c', 'z', 'g',
  KEY_LEFT_CTRL, KEY_LEFT_SHIFT, 'v', 'f', 'y'};

//special keys
char joystick_btn = 'm', forward = 'w', back = 's', right = 'd', left = 'a';
//#define EMULATE_CONTROLLER

#elif CURRENT_GAME == HALO
//main keys
char keys[SHIFTING_PINS_SIZE * INPUT_PINS_SIZE] = {
  KEY_ESC, '2', '3', '4', '5',
  'q', 'w', 'e', 'r', 't',
  'a','s', 'd', 'f', 'g',
  'z', 'x', 'c', 'v', 'b'};

//special keys
char joystick_btn = 'm', forward = '9', back = '8', right = '7', left = '6';

#elif CURRENT_GAME == THE_FINALS
//main keys
char keys[SHIFTING_PINS_SIZE * INPUT_PINS_SIZE] = {
  KEY_ESC, '2', '3', 'm', 'i',
  '1', '2', '3', '4', 'q',
  ' ', 'x', 'c', 'z', 'g',
  KEY_LEFT_CTRL, KEY_LEFT_SHIFT, 'v', 'f', 'y'};

//special keys
char joystick_btn = 'm', forward = 'w', back = 's', right = 'd', left = 'a';
//#define EMULATE_CONTROLLER
#endif

#ifdef EMULATE_CONTROLLER
#include <Joystick.h>

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD, 
  0, 0, // Button Count, Hat Switch Count 
  true, true, false,                           // X, Y, Z Axis 
  false, false, false,                         // Rx, Ry, Rz (Rotation) 
  false, false,                                // rudder, throttle 
  false, false, false);                        // accelerator, brake, steering 
#endif

bool previous_key_states[SHIFTING_PINS_SIZE * INPUT_PINS_SIZE];
bool forward_pressed, back_pressed, right_pressed, left_pressed;
bool previous_joystick_btn;

void setup() {
  Serial.begin(9600);
  Keyboard.begin();

#ifdef EMULATE_CONTROLLER
  Joystick.begin();
#endif
  
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
  for(byte s = 0; s < SHIFTING_PINS_SIZE; s++)
  {
    digitalWrite(shifting_pins[s], LOW);
    for(byte i = 0; i < INPUT_PINS_SIZE; i++)
    {
      byte loop_times = s * INPUT_PINS_SIZE + i;
      bool now_pressed = !digitalRead(input_pins[i]);
      bool previous_pressed = previous_key_states[loop_times];

#if 0
      Serial.print("s:");
      Serial.print(s);
      Serial.print(" i: ");
      Serial.print(i);
      Serial.print(" loop_times: ");
      Serial.println(loop_times);
      delay(100);
#endif

      if(now_pressed && !previous_pressed)
      {
        Keyboard.press(keys[loop_times]);

#if 0
        Joystick.setButton(loop_times, 1);
#endif
      }
      else if (!now_pressed && previous_pressed)
      {
        Keyboard.release(keys[loop_times]);

#if 0
        Joystick.setButton(loop_times, 0);
#endif
      }
      previous_key_states[loop_times] = now_pressed;
    }
    digitalWrite(shifting_pins[s], HIGH);
  }
}

void read_joystick_button() 
{ 
  bool now_joystick_btn = !digitalRead(joystick_button_pin);
  if(now_joystick_btn && !previous_joystick_btn) 
  {  
    Keyboard.press(joystick_btn);

#if 0
    Joystick.setButton(SHIFTING_PINS_SIZE * INPUT_PINS_SIZE, 1);
#endif
  } 
  else if(!now_joystick_btn && previous_joystick_btn)
  { 
    Keyboard.release(joystick_btn);

#if 0
    Joystick.setButton(SHIFTING_PINS_SIZE * INPUT_PINS_SIZE, 0);
#endif
  }
  previous_joystick_btn = now_joystick_btn;
} 

void read_joystick_keys()
{
  int dead_zone_press = 200, dead_zone_release = 100, mid_x = 553, mid_y = 543;

  int x_read = analogRead(VRX);
  int y_read = analogRead(VRY);
#if 0
  //Serial.println(!digitalRead(joystick_button_pin));
  Serial.print("x: ");
  Serial.println(x_read);
  Serial.print("y: ");
  Serial.println(y_read);
#endif

#ifdef EMULATE_CONTROLLER
  Joystick.setXAxis(x_read); 
  Joystick.setYAxis(y_read);
#else
  //forward
  bool now_forward_press = y_read < mid_y - dead_zone_press;
  bool now_forward_release = y_read > mid_y - dead_zone_release;
  if(now_forward_press && !forward_pressed)
  {
    Keyboard.press(forward);
    forward_pressed = true;
  }
  else if (now_forward_release && forward_pressed)
  {
    Keyboard.release(forward);
    forward_pressed = false;
  }

  //back
  bool now_back_press = y_read > mid_y + dead_zone_press;
  bool now_back_release = y_read < mid_y + dead_zone_release;
  if(now_back_press && !back_pressed)
  {
    Keyboard.press(back);
    back_pressed = true;
  }
  else if(now_back_release && back_pressed)
  {
    Keyboard.release(back);
    back_pressed = false;
  }

  //right
  bool now_right_press = x_read < mid_x - dead_zone_press;
  bool now_right_release = x_read > mid_x - dead_zone_release;
  if(now_right_press && !right_pressed)
  {
    Keyboard.press(right);
    right_pressed = true;
  }  
  else if(now_right_release && right_pressed)
  {
    Keyboard.release(right);
    right_pressed = false;
  }

  //left
  bool now_left_press = x_read > mid_x + dead_zone_press;
  bool now_left_release = x_read < mid_x + dead_zone_release;
  if(now_left_press && !left_pressed)
  {
    Keyboard.press(left);
    left_pressed = true;
  }
  else if (now_left_release && left_pressed)
  {
    Keyboard.release(left);
    left_pressed = false;
  }
#endif 
}

void loop() {
  read_main_buttons();
  read_joystick_keys();
  read_joystick_button();
  delay(1);
}
