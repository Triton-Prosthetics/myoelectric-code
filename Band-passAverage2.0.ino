// including Servo package and initializing the servo names
#include <Servo.h>
Servo Servo1; 
Servo Servo2;
Servo Servo3;
Servo Servo4;


// the number of points the moving average filter considers when calculating the average
// only need to change this constant to change how many points filter is affected by in code
#define AVERAGE_POINTS 25

//Global Variables
int sensorPin = A0;        //pin number to use the ADC
int sensorValue = 0;      //initialization of sensor variable

int servoPin = 9;
int servoPin2 =8;
int servoPin3 =7;
int servoPin4 =11;

int pos = 0;

int NOISE_THRESHOLD = 550;      //minimum threshold to consider a signal not noise
int TIME_THRESHOLD = 200;       //minimum signal average over last AVERAGE_POINTS threshold to consider a flex intentional

int HIGH_FLEX = 1;              //the output when the sensor is reading a high level flex
int ACTIVATION_FATIGUE = 200;   //the minimum time that must have elapsed to activate / deactivate the device again
bool activated = false;         //current activation state of device
int active_counter = 0;         //keeps track of time since device was last activated / deactivated

// allocate memory for the array of signals
float signalArray[AVERAGE_POINTS];
// initialize iterator for moving average filter
int iterator = 0;


// the setup routine runs once when you press reset:
void setup() {
  
  Serial.begin(9600);                   //setup of Serial module, 9600 bits/second  
  
  Servo1.write(0);
  Servo1.attach(servoPin);
  Servo4.write(0);
  Servo4.attach(servoPin4);
  Servo3.write(0);
  Servo3.attach(servoPin3);
  Servo2.write(0);
  Servo2.attach(servoPin2);


  
  // zero initialize the signal array so it starts off 'at rest'
  for(int index = 0; index < AVERAGE_POINTS; index++) {
    signalArray[index] = 0.0;
  }
}

// the loop routine runs over and over again forever:
void loop() {
  sensorValue = analogRead(sensorPin);    //read the sensor value using ADC
  // only add signal if it is not noise
  if(sensorValue > NOISE_THRESHOLD) {
    updateArray(sensorValue);
  } else {
    updateArray(0);
  }
  
  float filteredSignal = mean(signalArray);
  // reverse activation state of intentional flex detected
  if(active_counter > 200 && abs(filteredSignal) > TIME_THRESHOLD) {
    activated = !activated;
    if (pos==0) {
      pos=180;
    }
    else {
      pos=0;
    }
    Servo1.write(pos);
    Servo2.write(pos);
    Servo3.write(pos);
    Servo4.write(pos);
    active_counter = 0;
  }

  if(activated) {
    Serial.println(HIGH_FLEX);
  } else {
    Serial.println(0);
  } 

  active_counter++; 
  delay(20);                              //20ms delay for readability
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
