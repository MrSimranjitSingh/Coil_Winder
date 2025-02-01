void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  if (Serial.available() > 0) {
    rc = Serial.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    } else {
      receivedChars[ndx] = '\0';  // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}

void showNewNumber(int *value, const String &str = "") {
  if (newData) {
    newData = false;
    *value = atoi(receivedChars);
    if (str.length() > 0) {
      Serial.print(str);
    }
    Serial.println(receivedChars);
  }
}

void showNewNumber(float *value, const String &str = "") {
  if (newData) {
    newData = false;
    *value = atof(receivedChars);
    if (str.length() > 0) {
      Serial.print(str);
    }
    Serial.println(receivedChars);
  }
}


void getData(int *value) {
  while (*value == 0) {
    recvWithEndMarker();
    showNewNumber(value);
  }
}

void getData(float *value) {
  while (*value == 0) {
    recvWithEndMarker();
    showNewNumber(value);
  }
}

void getNewSpeed() {
  int value = 0;
  recvWithEndMarker();
  showNewNumber(&value, "New delay: ");
  if (value >= minDelay && value <= maxDelay) {
    microsbetweenSteps = value;
  }
}
