#define THROW_ERROR_IF_NOT_FAST  // Activate this to detect where ...Fast() functions are called with NON constant parameters and are therefore still slow.
#include <digitalWriteFast.h>
#include <math.h>


const byte x_step = 2;
const byte y_step = 3;
const byte z_step = 4;

const byte x_dir = 5;
const byte y_dir = 6;
const byte z_dir = 7;

const unsigned long x_spr = 51200;  //Steps per revolution = 200 * 32 * 8
const unsigned long y_spr = 6400;   //Steps per revolution = 200 * 32
const unsigned long z_spr = 6400;   //Steps per revolution = 200 * 32

const float spacing_mm = 0.01;                    // Spacing between turns. Manufacturer states wire tolerance is +/-0.05mm for 34awg wire
const float wire_diameter = 0.1016 + spacing_mm;  // Wire diameter in mm (this is how much the wire guide advances per rotation)

const float steps_per_mm = float(y_spr) / 8.0;  //2mm pitch 4 start lead screw (4*2). 800 steps = 1mm

const float steps_y_float = steps_per_mm * wire_diameter;
const uint16_t steps_y_int = (uint16_t)steps_y_float;

// Calculate the X motor steps (pipe rotation) for each step of Y-axis
// Each Y step corresponds to 8 X steps to complete one full pipe revolution for 6400 steps
const float steps_x_per_y_float = (float)x_spr / steps_y_float;  // Steps required for y per step of x
const uint16_t steps_x_per_y_int = (uint16_t)steps_x_per_y_float;

const float steps_x_remainder = steps_x_per_y_float - steps_x_per_y_int;  // Remaining fractional steps for x
const float steps_y_remainder = steps_y_float - steps_y_int;              // Remaining fractional steps for y

const uint16_t pulseWidthMicros = 5;  // microseconds

const uint16_t minDelay = 30;         //Minimum time between stepper motr steps
const uint16_t maxDelay = 5000;       //Maximum time between stepper motor steps
uint16_t microsbetweenSteps = 1000;   //Default delay
const uint16_t finalMicros = 200;     //Final delay/speed => reduceDelay()

const String s_enter = "enter";
const String s_dashes = "***************";

const byte numChars = 32;
char receivedChars[numChars];  // an array to store the received data
boolean newData = false;


struct MenuItem {
  String message;    // Message to display
  void (*action)();  // Function to call for menu action
};


// Function declarations
void windCoil();
void moveHeadMM();
void moveHeadSteps();
void unwindCoil();
void rotateCoil();
void windInductor();
void windSlotedCoil();

MenuItem menuItems[] = {
  { "Wind coil", windCoil },
  { "Wind inductor", windInductor },
  { "Wind Sloted Coil", windSlotedCoil },
  { "Move head (mm)", moveHeadMM },
  { "Move head (steps)", moveHeadSteps },
  { "Rotate coil", rotateCoil },
  { "Unwind coil", unwindCoil }
};

const int menuSize = sizeof(menuItems) / sizeof(menuItems[0]);

void setup() {

  Serial.begin(115200);

  pinModeFast(x_dir, OUTPUT);
  pinModeFast(x_step, OUTPUT);

  pinModeFast(y_dir, OUTPUT);
  pinModeFast(y_step, OUTPUT);

  pinModeFast(z_dir, OUTPUT);
  pinModeFast(z_step, OUTPUT);


  String str = s_dashes + "Data" + s_dashes;
  Serial.println("\r\n\r\n\r\n" + str);

  // Print variables for debugging
  Serial.println("Steps   X: " + String(x_spr));
  Serial.println("Steps   Y: " + String(steps_y_float, 4) + " (" + String(wire_diameter, 4) + "mm)");
  Serial.println("Steps X/Y: " + String(steps_x_per_y_float, 4));

  Serial.println(str);
}

void loop() {
  int choice = showMenu();
  executeMenuChoice(choice);
}


//This function automatically reduces delay overtime
void reduceDelay() {
  static unsigned long previousMillis = 0;  // Stores the last update time
  
  unsigned long currentMillis = millis();

  // Check if the time interval has passed
  if (currentMillis - previousMillis >= 500) {
    previousMillis = currentMillis;  // Update the last update time

    // Reduce global variable in steps down to the target value of 100
    if (microsbetweenSteps > finalMicros) {
      microsbetweenSteps -= 10;
      if (microsbetweenSteps < finalMicros) {
        microsbetweenSteps = finalMicros;  // Clamp to minimum value
      }
      //Serial.println("Global Variable: " + String(globalVariable));
    }
  }
}
