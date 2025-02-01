void wind_wire(byte x_dir, byte y_dir, uint16_t total_turns) {

  // Counter to track fractional steps for y
  static float fractional_counter_x = 0.0;
  static float fractional_counter_y = 0.0;
  static uint16_t turn_counter = 0;

  // Set the direction for stepper_x
  set_dir_x(x_dir);
  set_dir_y(y_dir);

  uint16_t maxMicros = maxDelay;
  uint16_t minMicros = microsbetweenSteps;

  for (uint16_t i = 0; i < total_turns; i++) {

    uint16_t y_motor_steps = steps_y_int;

    // Increment fractional_counter and move stepper_y if it exceeds one
    fractional_counter_y += steps_y_remainder;
    if (fractional_counter_y >= 1.0) {
      pulse_y(50);
      fractional_counter_y -= 1.0;  // Decrement by 1 to account for the whole step
    }

    for (uint16_t n = 0; n < y_motor_steps; n++) {
      
      // Move stepper_y
      pulse_y(0);

      // Step X multiple times for every step of y
      uint16_t x_motor_steps = steps_x_per_y_int;

      // Track fractional steps
      fractional_counter_x += steps_x_remainder;
      if (fractional_counter_x >= 1.0) {
        x_motor_steps++;
        fractional_counter_x -= 1.0;  // Decrement by 1 to account for the whole step
      }

      //Constant speed
      //stepMotor(x_step, x_motor_steps, microsbetweenSteps);


      //Code for acceleration and deceleration
      //If its first turn
      if (i == 0) {

        if (n == 0) {
          stepMotorAcc(x_motor_steps, minMicros, maxMicros);
        } else if (n + 1 != y_motor_steps) {
          stepMotor(x_step, x_motor_steps, microsbetweenSteps);
        } else if (n + 1 == y_motor_steps && i + 1 == total_turns) {  //If its first and last turn
          stepMotorDec(x_motor_steps, minMicros, maxMicros);
        }

      } else if (i != 0) {

        if (i + 1 != total_turns) {  //All other turns except first & last
          stepMotor(x_step, x_motor_steps, microsbetweenSteps);
        } else if (i + 1 == total_turns) {  //Last Turns
          if (n + 1 == y_motor_steps) {
            stepMotorDec(x_motor_steps, minMicros, maxMicros);
          } else {
            stepMotor(x_step, x_motor_steps, microsbetweenSteps);
          }
        }
      }
    }
    ++turn_counter;
    String str = "";
    if(turn_counter % 5 == 0){
      str = "\r\n";
    }else{
      str = "\t";
    }

    Serial.print("  Turns : " + String(turn_counter) + " , Y_frac: " + String(fractional_counter_y, 4) + str);

    
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
