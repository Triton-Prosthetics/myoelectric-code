// the number of points the moving average filter considers when calculating the average
// only need to change this constant to change how many points filter is affected by in code
#define AVERAGE_POINTS 25
#include <Servo.h> 
float signalArray[AVERAGE_POINTS];
const int analogInPin = A1;
int signalPin= A0; 
int servoPin = 8;
int tranPin = 11; //transistor
int previousValue = HIGH;
Servo Servo1;

//MECHANICS
// the setup routine runs once when you press reset:
void setup() {
  // zero initialize the signal array so it starts off 'at rest'
  for(int index = 0; index < AVERAGE_POINTS; index++) {
    signalArray[index] = 0.0;
  }
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Servo1.attach(servoPin);
  pinMode(tranPin, OUTPUT);
}

// allocate memory for the array of signals
// initialize iterator for moving average filter
int iterator = 0;




// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(signal);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
  // First attempt at implementing a moving average filter (Savitzky-Golay)
  // http://www.dspguide.com/CH15.PDF
  updateArray(voltage);
  float filteredSignal = mean(signalArray);
  // print out the value you read:
  Serial.println(filteredSignal); // use this variable for controling motor 

  // LOOP FOR SERVO MOTOR USING filteredSignal AS THE INPUT
  digitalWrite(tranPin, HIGH);
  sensorValue = analogRead(analogInPin);
  if( filteredSignal=0)
  {
    //hand closed
     digitalWrite(tranPin, LOW);
  }
  if(filteredSignal>0 && filteredSignal<4)
  {
    //hand opened 
     digitalWrite(tranPin, HIGH);
  }
  
}

/*
 * Updates the array of inputs of length AVERAGE_POINTS (the desired number of 
 * past data points to consider when constructing the mean) and handles its iterator.
 * 
 */
void updateArray(int input) {
  signalArray[iterator] = input;
  iterator += 1;
  // if iterator becomes longer than length of array, reset it to start at the beginning
  // since that is now the oldest data point
  if(iterator >= AVERAGE_POINTS) {
    iterator = 0;
  }
}

/*
 * Calculates the mean of the given signal array based off of AVERAGE_POINTS
 * 
 */
float mean(float inputArray[]) {
  float sum = 0.0;
  for(int index = 0; index < AVERAGE_POINTS; index++) {
    sum += inputArray[index];
  } 
  sum /= (float) AVERAGE_POINTS;
  return sum;
  }
 }
