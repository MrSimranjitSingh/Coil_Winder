void wind_wire(byte dir, uint16_t turns) {
  // Initialize variables for steps needed for x and y movements

  // Print variables for debugging
  Serial.println("\nSteps   X: " + String(x_spr));
  Serial.println("Steps   Y: " + String(steps_y_float, 4) + " (" + String(wire_diameter, 4) + "mm)");
  Serial.println("Steps X/Y: " + String(steps_x_per_y_float, 4));
  Serial.println("");


  // Counter to track fractional steps for y
  static float fractional_counter_x = 0.0;
  static float fractional_counter_y = 0.0;
  static uint16_t turn_counter = 0;

  // Set the direction for stepper_x
  set_dir_x(dir);
  set_dir_y(dir);

  for (uint16_t i = 0; i < turns; i++) {

    for (uint16_t n = 0; n < steps_y_int; n++) {
      // Move stepper_y
      pulse_y(0);

      // Move x multiple times for every step of y
      // because steps for x is larger than steps for y
      for (uint16_t m = 0; m < steps_x_per_y_int; m++) {
        pulse_x(microsbetweenSteps);
      }
      // Increment fractional_counter and move stepper_x if it exceeds one
      fractional_counter_x += steps_x_remainder;
      if (fractional_counter_x >= 1.0) {
        pulse_x(0);
        fractional_counter_x -= 1.0;  // Decrement by 1 to account for the whole step
      }
    }
    // Increment fractional_counter and move stepper_y if it exceeds one
    fractional_counter_y += steps_y_remainder;
    if (fractional_counter_y >= 1.0) {
      pulse_y(microsbetweenSteps);
      fractional_counter_y -= 1.0;  // Decrement by 1 to account for the whole step
    }

    turn_counter++;

    Serial.print("Turns : " + String(turn_counter));
    Serial.print(" , X fraction : " + String(fractional_counter_x));
    Serial.println(" , Y fraction : " + String(fractional_counter_y));

    //Every few turns move forward to compensate for helical angle
    /*if (turn_counter % 7 == 0) {
      float s = steps_y_float * (0.75);
      for (uint16_t m = 0; m < s; m++) {
        pulse_y(microsbetweenSteps);
      }
      Serial.println("Moving Forward...");
    }*/
  }
}


void unwind_wire(byte dir, uint16_t turns) {

  // Counter to track fractional steps for y
  static float fractional_counter_y = 0.0;
  static uint16_t turn_counter = 0;

  static const float steps_y_float = steps_per_mm * wire_diameter;
  static const float steps_y_remainder = steps_y_float - (int)steps_y_float;  // Remaining fractional steps for y
  Serial.println("");

  set_dir_x(dir);
  set_dir_y(dir);

  const uint16_t max_delay_us = 5000;
  uint16_t smallest_us = 50;
  uint16_t change_us = 5;

  for (uint16_t i = 0; i < turns; i++) {

    unsigned long steps_left = z_spr;
    unsigned long num_steps = 0;

    //Acceleration
    /*for (uint16_t n = max_delay_us; n >= smallest_us; n -= change_us) {
      pulse_z(n);
      num_steps++;
    }
    steps_left -= 2 * num_steps;
    */

    //Constant Speed
    for (unsigned long n = 0; n < steps_left; n++) {
      pulse_z(smallest_us);
      num_steps++;
    }

    //Decelration
    /*for (uint16_t n = smallest_us; n <= max_delay_us; n += change_us) {
      pulse_z(n);
      num_steps++;
    }*/

    for (uint16_t m = 0; m < (uint16_t)steps_y_float; m++) {
      pulse_y(microsbetweenSteps);
    }

    // Increment fractional_counter and move stepper_y if it exceeds one
    fractional_counter_y += steps_y_remainder;
    if (fractional_counter_y >= 1.0) {
      pulse_y(microsbetweenSteps);
      fractional_counter_y -= 1.0;  // Decrement by 1 to account for the whole step
    }

    turn_counter++;

    Serial.print("Turns : " + String(turn_counter));
    Serial.println(" , Fraction Y: " + String(fractional_counter_y));
  }
}

void rotate(uint16_t delay) {
  while (1) {
    pulse_x(delay);
  }
}


void move_y(byte dir, float distance) {

  set_dir_y(!dir);

  unsigned long total_steps = steps_per_mm * distance;
  Serial.println("Steps Y = " + String(total_steps));
  for (unsigned long n = 0; n < total_steps; n++) {
    pulse_y(microsbetweenSteps);
  }
}


void move_y_steps(byte dir, unsigned long steps) {

  set_dir_y(!dir);

  for (unsigned long n = 0; n < steps; n++) {
    pulse_y(microsbetweenSteps);
  }
}
