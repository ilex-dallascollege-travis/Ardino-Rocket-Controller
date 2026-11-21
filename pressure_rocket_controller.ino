#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

// Known pins from your Tinkercad circuit/starter code
const int TARGET_POT_PIN   = A0; // Potentiometer: "to set target pressure (psi) [A0]"
const int LED_PINS[]       = {13, 12, 8, 4};
const int NUM_LEDS         = 4;

// Adjust these pins once you check the rest of the Tinkercad wiring
const int CURRENT_POT_PIN  = A1; // Simulated current pressure sensor input
const int COMPRESSOR_PIN   = 6;  // Compressor control output
const int LAUNCH_PIN       = 7;  // Rocket launch trigger output

const int COUNTDOWN_SECONDS = 5;

bool parseCustomNumber(const char* rawInput, long* outValue) {
  while (*rawInput && isspace((unsigned char)*rawInput)) {
    rawInput++;
  }
  if (*rawInput == '\0') return false;

  int base = 2; // Default is binary when no 0x/0X prefix is given
  const char* numStart = rawInput;

  if (rawInput[0] == '0' && (rawInput[1] == 'x' || rawInput[1] == 'X')) {
    base = 16; // Hexadecimal when prefixed with 0x or 0X
    numStart = rawInput + 2;
    if (*numStart == '\0') return false;
  }

  char* endPtr = nullptr;
  errno = 0;
  long parsed = strtol(numStart, &endPtr, base);

  while (*endPtr && isspace((unsigned char)*endPtr)) {
    endPtr++;
  }

  if (errno != 0 || endPtr == numStart || *endPtr != '\0') {
    return false;
  }

  *outValue = parsed;
  return true;
}

void displayBinaryOnLeds(int value) {
  for (int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(LED_PINS[i], bitRead(value, i));
  }
}

int getTargetPressure() {
  // Check Serial first if the user enters a binary (no prefix) or 0x hex value
  if (Serial.available() > 0) {
    String line = Serial.readStringUntil('\n');
    line.trim();
    long parsedVal = 0;
    if (line.length() > 0 && parseCustomNumber(line.c_str(), &parsedVal) && parsedVal >= 0) {
      Serial.print(F("Target pressure set via input to: "));
      Serial.println(parsedVal);
      return (int)parsedVal;
    } else if (line.length() > 0) {
      Serial.println(F("Invalid number. Enter binary (e.g. 1010) or hex with 0x (e.g. 0x0A)."));
    }
  }

  // Otherwise read target pressure from potentiometer A0 (scaled to 0-100 psi)
  int potRaw = analogRead(TARGET_POT_PIN);
  return map(potRaw, 0, 1023, 0, 100);
}

int getCurrentPressure() {
  int sensorRaw = analogRead(CURRENT_POT_PIN);
  return map(sensorRaw, 0, 1023, 0, 100);
}

void runRocketCountdown(int seconds) {
  Serial.println(F("Target pressure reached! Compressor OFF. Starting rocket countdown..."));
  for (int t = seconds; t >= 0; t--) {
    displayBinaryOnLeds(t);
    Serial.print(F("T-"));
    Serial.println(t);
    delay(1000);
  }

  Serial.println(F("LAUNCH!"));
  digitalWrite(LAUNCH_PIN, HIGH);
  delay(1000);
  digitalWrite(LAUNCH_PIN, LOW);
}

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }
  pinMode(CURRENT_POT_PIN, INPUT);
  pinMode(COMPRESSOR_PIN, OUTPUT);
  pinMode(LAUNCH_PIN, OUTPUT);
  digitalWrite(COMPRESSOR_PIN, LOW);
  digitalWrite(LAUNCH_PIN, LOW);

  Serial.println(F("System ready. Enter target in binary (no prefix) or hex (0x...), or use A0 pot."));
}

void loop() {
  int targetPressure = getTargetPressure();
  int currentPressure = getCurrentPressure();

  // Display current pressure (0-15 range) on the 4 LEDs during regulation
  displayBinaryOnLeds(constrain(currentPressure, 0, 15));

  if (currentPressure < targetPressure) {
    digitalWrite(COMPRESSOR_PIN, HIGH);
  } else {
    digitalWrite(COMPRESSOR_PIN, LOW);
    if (targetPressure > 0) {
      runRocketCountdown(COUNTDOWN_SECONDS);
    }
  }

  delay(200);
}
