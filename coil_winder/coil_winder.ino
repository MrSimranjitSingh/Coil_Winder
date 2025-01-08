#define THROW_ERROR_IF_NOT_FAST  // Activate this to detect where ...Fast() functions are called with NON constant parameters and are therefore still slow.
#include <digitalWriteFast.h>


const byte x_step = 2;
const byte y_step = 3;
const byte z_step = 4;
const byte x_dir = 5;
const byte y_dir = 6;
const byte z_dir = 7;

const unsigned long x_spr = 51200;     //Steps per revolution = 200 * 32 * 8
const unsigned long y_spr = 200 * 32;  //Steps per revolution
//const unsigned long z_spr = 19200;     //Steps per revolution = 200 * 32 * 3
const unsigned long z_spr = 6400;     //Steps per revolution = 200 * 32 * 3

const uint16_t pulseWidthMicros = 5;     // microseconds
const uint16_t microsbetweenSteps = 25;  // microseconds - or try 1000 for slower steps

//const float wire_diameter = 0.1601;             // in mm
const float wire_diameter = 0.1900;             // in mm
const float steps_per_mm = float(y_spr) / 8.0;  //2mm pitch 4 start lead screw (4*2). 25 steps = 1mm
const float steps_y_float = steps_per_mm * wire_diameter;
const uint16_t steps_y_int = (uint16_t)steps_y_float;

const float steps_x_per_y_float = (float)x_spr / steps_y_float;  // Steps required for y per step of x
const uint16_t steps_x_per_y_int = (uint16_t)steps_x_per_y_float;
const float steps_x_remainder = steps_x_per_y_float - steps_x_per_y_int;  // Remaining fractional steps for x
const float steps_y_remainder = steps_y_float - steps_y_int;              // Remaining fractional steps for y

const String s_enter = "enter";


const byte numChars = 32;
char receivedChars[numChars];  // an array to store the received data
boolean newData = false;
int dataNumber = 0;


void setup() {

  Serial.begin(115200);

  pinModeFast(x_dir, OUTPUT);
  pinModeFast(x_step, OUTPUT);

  pinModeFast(y_dir, OUTPUT);
  pinModeFast(y_step, OUTPUT);

  pinModeFast(z_dir, OUTPUT);
  pinModeFast(z_step, OUTPUT);
}

void loop() {

  int menuChoice = showMenu();
  const String msg_1 = s_enter + " number of turns";
  const String msg_2 = s_enter + " distance (mm) to move: ";
  const String msg_3 = s_enter + " steps to move: ";
  const String msg_4 = msg_1 + " to unwind: ";
  const String msg_5 = s_enter + " delay (us) (5-1000): ";
  const String default_msg = "\nPlease choose a valid selection";


  while (1) {
    switch (menuChoice) {
      case 1:
        // temp sensor code goes here
        Serial.print(msg_1 + ": ");
        getData();
        if (dataNumber > 0) {
          wind_wire(1, dataNumber);
        }
        dataNumber = 0;
        break;

      case 2:
        Serial.print(msg_2);
        getData();
        if (dataNumber > 0) {
          move_y(1, dataNumber);
        } else if (dataNumber < 0) {
          move_y(0, -dataNumber);
        }
        dataNumber = 0;
        break;
      case 3:
        Serial.print(msg_3);
        getData();
        if (dataNumber > 0) {
          move_y_steps(1, dataNumber);
        } else if (dataNumber < 0) {
          move_y_steps(0, -dataNumber);
        }
        dataNumber = 0;
        break;
      case 4:
        Serial.print(msg_4);
        getData();
        if (dataNumber > 0) {
          unwind_wire(0, dataNumber);
        }
        dataNumber = 0;
        break;
      case 5:
        Serial.print(msg_5);
        getData();
        if (dataNumber > 0) {
          rotate(dataNumber);
        }
        dataNumber = 0;
        break;
      default:
        Serial.println(default_msg);
        menuChoice = showMenu();
        break;
    }
  }
}
