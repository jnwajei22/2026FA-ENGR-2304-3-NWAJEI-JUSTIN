/* 2026FA-ENGR-2304-3-P09-NWAJEI-JUSTIN */
/*
 * ENGR 2304 | P09: Project Icarus, Arduino simulation
 *
 * Tinkercad ONLY: LED indicators stand in for the compressor and launcher.
 * This sketch does not control any real pressure equipment, igniter,
 * projectile, valve, or launch mechanism.
 *
 * A0 = potentiometer wiper, representing pressure (0 to 120 PSI)
 * D8 = compressor-status LED through 220-ohm resistor to GND
 * D9 = simulated-launch LED through 220-ohm resistor to GND
 * D7 = abort push button to GND; INPUT_PULLUP is enabled
 *
 * Serial Monitor: 9600 baud, Newline line ending.
 * Numeric targets without 0x are BINARY. With 0x, they are HEXADECIMAL.
 * Examples: 1010000 = 80 PSI; 0x50 = 80 PSI; 101010 = 42 PSI.
 */

#include <Arduino.h>

const byte PRESSURE_PIN = A0;
const byte COMPRESSOR_LED = 8;
const byte LAUNCH_LED = 9;
const byte ABORT_BUTTON = 7;

const unsigned long FILL_TIMEOUT_MS = 60000UL;
const unsigned long STATUS_INTERVAL_MS = 1000UL;
const unsigned long COUNTDOWN_INTERVAL_MS = 1000UL;
const unsigned long COMPLETE_LED_MS = 2000UL;
const int MAX_PRESSURE_PSI = 120;
const int PRESSURE_TOLERANCE_PSI = 5;

enum State { WAITING_FOR_TARGET, FILLING, COUNTING_DOWN, COMPLETE, ABORTED };
State state = WAITING_FOR_TARGET;

int targetPsi = 0;
int secondsRemaining = 10;
unsigned long fillStartMs = 0;
unsigned long lastStatusMs = 0;
unsigned long lastCountMs = 0;
unsigned long completeStartMs = 0;
char serialBuffer[24] = {0};
byte serialLength = 0;
bool inputOverflow = false;

int readPressurePsi() {
  return (int)((long)analogRead(PRESSURE_PIN) * MAX_PRESSURE_PSI / 1023L);
}

void outputsOff() {
  digitalWrite(COMPRESSOR_LED, LOW);
  digitalWrite(LAUNCH_LED, LOW);
}

void promptForTarget() {
  Serial.println(F("Enter target 10-100 PSI in BINARY (e.g. 1010000 = 80)"));
  Serial.println(F("or hexadecimal with 0x prefix (e.g. 0x50 = 80)."));
}

// Read a positive target: binary by default; hex only with a 0x prefix.
// Reject malformed strings and values outside 10..100 PSI.
bool parsePressure(const char* token, int* valueOut) {
  while (*token == ' ' || *token == '\t') ++token;
  int base = 2;
  if (token[0] == '0' && (token[1] == 'x' || token[1] == 'X')) {
    base = 16;
    token += 2;
  }
  if (*token == '\0') return false;

  unsigned int value = 0;
  bool seenDigit = false;
  while (*token != '\0' && *token != '\r' && *token != ' ' && *token != '\t') {
    char c = *token++;
    int digit;
    if (c >= '0' && c <= '9') digit = c - '0';
    else if (c >= 'a' && c <= 'f') digit = 10 + c - 'a';
    else if (c >= 'A' && c <= 'F') digit = 10 + c - 'A';
    else return false;
    if (digit >= base) return false;
    // Early bound prevents overflow on arbitrarily long input.
    if (value > 100U / (unsigned int)base) return false;
    value = value * (unsigned int)base + (unsigned int)digit;
    if (value > 100U) return false;
    seenDigit = true;
  }
  while (*token == ' ' || *token == '\t' || *token == '\r') ++token;
  if (*token != '\0' || !seenDigit || value < 10U) return false;
  *valueOut = (int)value;
  return true;
}

void abortRun(const __FlashStringHelper* reason) {
  outputsOff();
  state = ABORTED;
  Serial.print(F("ABORTED: "));
  Serial.println(reason);
  Serial.println(F("Release ABORT, then enter a new target to restart."));
}

void startFill(int target) {
  targetPsi = target;
  fillStartMs = millis();
  lastStatusMs = millis();
  digitalWrite(LAUNCH_LED, LOW);
  digitalWrite(COMPRESSOR_LED, HIGH);
  state = FILLING;
  Serial.print(F("Target accepted: "));
  Serial.print(targetPsi);
  Serial.println(F(" PSI; compressor LED ON."));
}

void startCountdown() {
  digitalWrite(COMPRESSOR_LED, LOW);
  state = COUNTING_DOWN;
  secondsRemaining = 10;
  lastCountMs = millis();
  Serial.println(F("Target reached; compressor LED OFF."));
  Serial.println(F("Simulated countdown: 10"));
}

void processSerialLine() {
  serialBuffer[serialLength] = '\0';
  if (inputOverflow) {
    Serial.println(F("Input too long; try again."));
  } else if (state == WAITING_FOR_TARGET || state == ABORTED) {
    int parsedTarget;
    if (digitalRead(ABORT_BUTTON) == LOW) {
      Serial.println(F("Release ABORT button first."));
    } else if (parsePressure(serialBuffer, &parsedTarget)) {
      startFill(parsedTarget);
    } else {
      Serial.println(F("Invalid target. Use binary or 0x-prefixed hex; range 10-100 PSI."));
      promptForTarget();
    }
  } else {
    Serial.println(F("Run active; use ABORT to reset before changing the target."));
  }
  serialLength = 0;
  inputOverflow = false;
}

void pollSerialNonblocking() {
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    if (c == '\n' || c == '\r') {
      if (serialLength > 0 || inputOverflow) processSerialLine();
    } else if (serialLength < sizeof(serialBuffer) - 1) {
      serialBuffer[serialLength++] = c;
    } else {
      inputOverflow = true;
    }
  }
}

void setup() {
  pinMode(COMPRESSOR_LED, OUTPUT);
  pinMode(LAUNCH_LED, OUTPUT);
  pinMode(ABORT_BUTTON, INPUT_PULLUP);
  outputsOff();
  Serial.begin(9600);
  Serial.println(F("ENGR 2304 P09 - Project Icarus simulation"));
  promptForTarget();
}

void loop() {
  pollSerialNonblocking();
  unsigned long now = millis();

  if ((state == FILLING || state == COUNTING_DOWN || state == COMPLETE)
      && digitalRead(ABORT_BUTTON) == LOW) {
    abortRun(F("abort button pressed"));
    return;
  }

  switch (state) {
    case WAITING_FOR_TARGET:
    case ABORTED:
      break;

    case FILLING:
      if (now - fillStartMs >= FILL_TIMEOUT_MS) {
        abortRun(F("pressure target not reached in 60 seconds"));
      } else if (readPressurePsi() >= targetPsi) {
        startCountdown();
      } else if (now - lastStatusMs >= STATUS_INTERVAL_MS) {
        lastStatusMs = now;
        Serial.print(F("Pressure: "));
        Serial.print(readPressurePsi());
        Serial.print(F(" / "));
        Serial.print(targetPsi);
        Serial.println(F(" PSI; compressor LED ON"));
      }
      break;

    case COUNTING_DOWN:
      if (readPressurePsi() + PRESSURE_TOLERANCE_PSI < targetPsi) {
        abortRun(F("pressure dropped below safe simulated threshold"));
      } else if (now - lastCountMs >= COUNTDOWN_INTERVAL_MS) {
        lastCountMs += COUNTDOWN_INTERVAL_MS;
        --secondsRemaining;
        Serial.print(F("Countdown: "));
        Serial.println(secondsRemaining);
        if (secondsRemaining == 0) {
          digitalWrite(LAUNCH_LED, HIGH);
          completeStartMs = now;
          state = COMPLETE;
          Serial.println(F("SIMULATED LAUNCH: indicator LED ON; no actuator attached"));
        }
      }
      break;

    case COMPLETE:
      if (now - completeStartMs >= COMPLETE_LED_MS) {
        outputsOff();
        state = WAITING_FOR_TARGET;
        Serial.println(F("Simulation finished."));
        promptForTarget();
      }
      break;
  }
}
