unsigned long time = 0;

const char echoLeft = 9;
const char trigLeft = 10;
const char echoRight = 11;
const char trigRight = 12;

const char leftIndex = 0;
const char rightIndex = 1;

long readUltrasonic(const char side) {
  char echo;
  char trig;

  switch (side) {
    case 0:
      echo = echoLeft;
      trig = trigLeft;
    case 1:
      echo = echoRight;
      trig = trigRight;
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

  Serial.begin(9600);
}

void loop() {
  time = millis();
  long distanceLeft = readUltrasonic(leftIndex);
  long distanceRight = readUltrasonic(rightIndex);

  Serial.print(time);
  Serial.print(",");
  Serial.print(distanceLeft);
  Serial.print(",");
  Serial.print(distanceRight);
  Serial.println();
}

// https://github.com/microsoft/vscode-arduino/issues/1651