// 7-segment display digit encoding
int num_array[10][7] = {
  {1,1,1,1,1,1,0},  // 0
  {0,1,1,0,0,0,0},  // 1
  {1,1,0,1,1,0,1},  // 2
  {1,1,1,1,0,0,1},  // 3
  {0,1,1,0,0,1,1},  // 4
  {1,0,1,1,0,1,1},  // 5
  {1,0,1,1,1,1,1},  // 6
  {1,1,1,0,0,0,0},  // 7
  {1,1,1,1,1,1,1},  // 8
  {1,1,1,0,0,1,1}   // 9
};

// Define segment pins
int segPins[] = {2, 3, 4, 5, 6, 7, 8};

void setup() {
  // Set all segment pins as OUTPUT
  for (int i = 0; i < 8; i++) {
    pinMode(segPins[i], OUTPUT);
  }
}

void loop() {
  // Countdown from 9 to 0
  for (int i = 9; i >= 0; i--) {
    // Directly write the number to 7-segment display
    for (int j = 0; j < 7; j++) {
      digitalWrite(segPins[j], num_array[i][j]);
    }
    delay(1000); // Wait 1 second
  }
  delay(3000); // Pause 3 seconds before restarting
}