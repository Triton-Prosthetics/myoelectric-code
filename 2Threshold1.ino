
#include <Servo.h>
Servo myservo;
int sensorPin = A1;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
float sensorValue = 0;  // variable to store the value coming from the sensor
int pos = 0;
int temp = 0;
double threshold2 = 5000;
double threshold1 = 1000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  myservo.attach(9); 
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorValue = pow(float(analogRead(sensorPin)),2);
  myservo.write(pos);
  //Serial.println(sensorValue);
  if (sensorValue >= threshold2 && pos < 180) {         // Highest Threshold for highest contraction
    digitalWrite(ledPin,HIGH);
    pos = pos+3; 
    Serial.println(sensorValue);
    myservo.write(pos);
    
  }
   else if (sensorValue >= threshold1 && sensorValue > temp && pos < 180) {    // Lowest Threshold for middle contraction
    digitalWrite(ledPin,HIGH);
    pos = pos+2; 
    Serial.println(sensorValue);
    myservo.write(pos);
    
  }
  else if (sensorValue < threshold2 && sensorValue <= temp && pos > 0){                  // Low or no contraction
    digitalWrite(ledPin,LOW);
    pos = pos-3;
    myservo.write(pos);
    Serial.println(sensorValue);
  }
  delay(15);
  temp = sensorValue;
}
