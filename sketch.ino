// AceSorting - Version: Latest 
#include <KickSort.h>

/* I N I T I A L    D E F I N E S */
#define echoPin  9              // Attach pin D9 Arduino to pin Echo of HC-SR04
#define trigPin 10              // Attach pin D10 Arduino to pin Trig of HC-SR04

unsigned long currentMillis;

const unsigned int WINDOW_SIZE = 5;
int window[WINDOW_SIZE];
unsigned int dataCount = 0;
int threshold = 20; // allowable difference for outlier filter

// int maxDistance = 200; // cm


int readUltrasonic() {
  long duration;                // Variable for the duration of sound wave travel
  int distance;                 // Variable for the distance measurement

  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  // Speed of sound wave divided by 2 (go and back)
  distance = duration * 0.034 / 2;
  
  // distance = distance > maxDistance ? maxDistance : distance;
  // populate the window if not yet full
  if (dataCount < WINDOW_SIZE) {
    window[dataCount] = distance;
    dataCount++;
    return -1;
  } else {
    for (int i = 0; i < WINDOW_SIZE - 1; i++) {
      window[i] = window[i + 1];
    }
    window[WINDOW_SIZE - 1] = distance;
    int median;
    if (isOutlier(distance, median)) {
      return median;
    } else {
      return distance;
    }
  }
}

void setup() {
  pinMode(trigPin, OUTPUT);     // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT);      // Sets the echoPin as an INPUT

  Serial.begin(9600);           // Configure and start Serial Communication
}

bool isOutlier(const int distance, int & median) {
  int toSort[WINDOW_SIZE];
  memcpy(toSort, window, sizeof(window));
  KickSort<int>::bubbleSort(toSort, WINDOW_SIZE);
  median = toSort[WINDOW_SIZE >> 1]; // middle index
  return abs(toSort[WINDOW_SIZE-1] - median) > threshold;
}

void loop() {
  // put your main code here, to run repeatedly:
  // Check the forward button signal
  currentMillis = millis();
  int distance = readUltrasonic();
  delay(20);
  // publish
  Serial.print(currentMillis);
  Serial.print(",");
  Serial.print(distance);
  Serial.print(",-1");
  Serial.print(",-1");
  Serial.print(",-1");
  Serial.println();
}
