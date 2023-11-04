unsigned long time = 0;

const char button = 4;
const char echoLeft = 9;
const char trigLeft = 10;
const char echoRight = 11;
const char trigRight = 12;

const char leftIndex = 0;
const char rightIndex = 1;

unsigned long debounceDuration = 50; // millis
unsigned long lastTimeButtonStateChanged = 0;
byte lastButtonState = LOW;

long readUltrasonic(const char side) {
  char echo;
  char trig;

  switch (side) {
    case 0:
      echo = echoLeft;
      trig = trigLeft;
      break;
    case 1:
      echo = echoRight;
      trig = trigRight;
      break;
  }

  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  return pulseIn(echo, HIGH);
}

void setup() {
  pinMode(trigLeft, OUTPUT);
  pinMode(trigRight, OUTPUT);
  pinMode(echoLeft, INPUT);
  pinMode(echoRight, INPUT);
  pinMode(button, INPUT_PULLUP);

  Serial.begin(9600);
}

bool isButtonPressed() {
  bool result = false;
    if (millis() - lastTimeButtonStateChanged > debounceDuration) {
      byte buttonState = digitalRead(button);
      if (buttonState != lastButtonState) {
      lastTimeButtonStateChanged = millis();
      lastButtonState = buttonState;
      if (buttonState == LOW) {
        result = true;
      }
    }
  }
  return result;
}

void loop() {
  time = millis();
  long distanceLeft = readUltrasonic(leftIndex);
  long distanceRight = readUltrasonic(rightIndex);
  // delay(20);
  bool isPressed = isButtonPressed();
  Serial.print(time);
  Serial.print(",");
  Serial.print(distanceLeft);
  Serial.print(",");
  Serial.print(distanceRight);
  Serial.print(",");
  Serial.print(isPressed);
  Serial.println();
}

// https://github.com/microsoft/vscode-arduino/issues/1651