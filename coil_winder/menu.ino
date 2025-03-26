int showMenu() {

  Serial.println(s_dashes + "Menu" + s_dashes);

  for (int i = 0; i < menuSize; i++) {
    Serial.println(String(i + 1) + ". " + menuItems[i].message);
  }
  Serial.println(s_dashes + "Menu" + s_dashes);

  Serial.print(s_enter + " choice: ");
  while (Serial.available() == 0) {}
  String str = Serial.readStringUntil('\n');
  int menuChoice = str.toInt();
  Serial.println(menuChoice);

  return menuChoice - 1;  // Adjust for 0-based index
}

void executeMenuChoice(int choice) {
  if (choice >= 0 && choice < menuSize) {
    menuItems[choice].action();
  } else {
    Serial.println("Invalid choice!");
  }
}

void getAndProcessData(const String &prompt, void (*process)(int)) {
  Serial.print(prompt);
  int value = 0;
  getData(&value);
  process(value);
}

// Menu actions
void windCoil() {
  getAndProcessData(s_enter + " number of turns: ", [](int value) {
    if (value > 0) wind_wire(1, 0, value);
    else if (value < 0) wind_wire(1, 1, -value);
  });
}

void moveHeadMM() {
  getAndProcessData(s_enter + " distance (mm) to move: ", [](int value) {
    if (value > 0) move_y(1, value);
    else if (value < 0) move_y(0, -value);
  });
}

void moveHeadSteps() {
  getAndProcessData(s_enter + " steps to move: ", [](int value) {
    if (value > 0) move_y_steps(1, value);
    else if (value < 0) move_y_steps(0, -value);
  });
}

void unwindCoil() {
  getAndProcessData(s_enter + " number of turns to unwind: ", [](int value) {
    if (value > 0) unwind_wire(0, value);
  });
}

void rotateCoil() {
  getAndProcessData(s_enter + " delay (us) (5-1000): ", [](int value) {
    if (value > 0) rotate(value);
  });
}

void windSlotedCoil() {
  float height = 0;
  float length = 0;
  int layers = 0;
  int direction = 1;

  /*
    Coil Shape:
              Length
            |-----
            |   /
    height  |  /
            | /
            |/
  */

  Serial.print(s_enter + " inductor height (mm): ");
  getData(&height);
  Serial.print(s_enter + " inductor length (mm): ");
  getData(&length);

  for (float i = height; i > wire_diameter; i -= wire_diameter) {

    // Calculate the turns at every possible point of height
    float distance = calculateDistance(i, height, length);
    float turns = distance / wire_diameter;

    if (turns >= 1.0) {
      Serial.println("\r\nDistance: " + String(distance, 4) + "   Turns: " + String(round(turns)));
      if (direction == 1) wind_wire(1, 0, round(turns));
      else wind_wire(1, 1, round(turns));
      direction *= -1;
    }
  }
}

void windInductor() {
  float length = 0;
  int layers = 0;

  Serial.print(s_enter + " inductor length (mm): ");
  getData(&length);
  Serial.print(s_enter + " number of layers: ");
  getData(&layers);

  float turns = length / wire_diameter;
  Serial.print("Turns / Layer: ");
  Serial.println(turns);

  for (uint8_t i = 0; i < layers; i++) {
    if (turns > 0) wind_wire(1, 0, turns);
    else wind_wire(1, 1, -turns);
    turns *= -1;
  }
}

// Function to calculate the perpendicular distance
float calculateDistance(float x, float b, float h) {
  if (x < 0 || x > b) {
    Serial.println("Error: x is out of range (0 to b).");
    return -1;  // Invalid input
  }
  // Formula for perpendicular distance
  return (h * abs(x - b)) / sqrt(h * h + 1);
}