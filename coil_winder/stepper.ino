void pulse_z(unsigned long micros) {
  digitalWriteFast(z_step, HIGH);
  delayMicroseconds(pulseWidthMicros);  // this line is probably unnecessary
  digitalWriteFast(z_step, LOW);
  delayMicroseconds(micros);
}

void pulse_y(unsigned long micros) {
  digitalWriteFast(y_step, HIGH);
  delayMicroseconds(pulseWidthMicros);  // this line is probably unnecessary
  digitalWriteFast(y_step, LOW);
  delayMicroseconds(micros);
}

void pulse_x(unsigned long micros) {
  digitalWriteFast(x_step, HIGH);
  delayMicroseconds(pulseWidthMicros);  // this line is probably unnecessary
  digitalWriteFast(x_step, LOW);
  delayMicroseconds(micros);
}

void stepMotor(int stepPin, unsigned long steps, unsigned long micro_s) {

  unsigned long currentMicros = 0;

  for (unsigned long i = 0; i < steps; i++) {
    digitalWriteFast(stepPin, HIGH);      // Send step pulse
    delayMicroseconds(pulseWidthMicros);  // Delay for stepper speed control
    digitalWriteFast(stepPin, LOW);
    delayMicroseconds(micro_s);
  }
  getNewSpeed();
  reduceDelay();
}

void stepMotorAcc(uint16_t accelSteps, uint32_t minMicros, uint32_t maxMicros) {
  uint32_t stepDelay = maxMicros;  // Initial delay for acceleration

  // Acceleration phase
  for (uint16_t i = 0; i < accelSteps; i++) {
    pulse_x(0);  // Send pulse to move motor
    delayMicroseconds(stepDelay);

    // Gradually reduce delay to increase speed
    if (stepDelay > minMicros) {
      stepDelay -= (maxMicros - minMicros) / accelSteps;
    }
  }
}

void stepMotorDec(uint16_t decelSteps, uint32_t minMicros, uint32_t maxMicros) {
  uint32_t stepDelay = minMicros;

  // Deceleration phase
  for (uint16_t i = 0; i < decelSteps; i++) {
    pulse_x(0);
    delayMicroseconds(stepDelay);

    // Gradually increase delay to decrease speed
    if (stepDelay < maxMicros) {
      stepDelay += (maxMicros - minMicros) / decelSteps;
    }
  }
}

void set_dir_x(uint8_t dir) {
  if (dir == 1) {
    digitalWriteFast(x_dir, HIGH);
  } else {
    digitalWriteFast(x_dir, LOW);
  }
}

void set_dir_y(uint8_t dir) {
  if (dir == 1) {
    digitalWriteFast(y_dir, LOW);
  } else {
    digitalWriteFast(y_dir, HIGH);
  }
}

void set_dir_z(uint8_t dir) {
  if (dir == 1) {
    digitalWriteFast(z_dir, HIGH);
  } else {
    digitalWriteFast(z_dir, LOW);
  }
}