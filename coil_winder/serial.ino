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

void showNewNumber() {
  if (newData == true) {
    dataNumber = 0;                    // new for this version
    dataNumber = atoi(receivedChars);  // new for this version
    Serial.println(receivedChars);
    //Serial.print("Data as Number ... ");    // new for this version
    //Serial.println(dataNumber);     // new for this version

    newData = false;
  }
}

void getData() {
  while (dataNumber == 0) {
    recvWithEndMarker();
    showNewNumber();
  }
}


int showMenu() {
  String s0 = "wind coil";
  String s1 = "move head ";
  Serial.println("1. " + s0);
  Serial.println("2. " + s1 + "(mm)");
  Serial.println("3. " + s1 + String(steps_y_float) + " (steps_y)");
  Serial.println("4. un" + s0);
  Serial.println("5. rotate coil");
  Serial.print(s_enter + ": ");
  while (Serial.available() == 0) {}
  String str = Serial.readStringUntil('\n');
  int menuChoice = str.toInt();
  Serial.println(menuChoice);
  return menuChoice;
}