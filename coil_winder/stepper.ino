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


void set_dir_x(uint8_t dir){
    if (dir == 1) {
    digitalWriteFast(x_dir, HIGH);
  } else {
    digitalWriteFast(x_dir, LOW);
  }
}

void set_dir_y(uint8_t dir){
    if (dir == 1) {
    digitalWriteFast(y_dir, LOW);
  } else {
    digitalWriteFast(y_dir, HIGH);
  }
}

void set_dir_z(uint8_t dir){
    if (dir == 1) {
    digitalWriteFast(z_dir, HIGH);
  } else {
    digitalWriteFast(z_dir, LOW);
  }
}