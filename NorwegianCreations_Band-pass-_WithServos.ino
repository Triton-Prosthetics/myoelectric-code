#include <Servo.h>
#include <time.h>

//Global Variables
int sensorPin = 0;        //pin number to use the ADC
int sensorValue = 0;      //initialization of sensor variable, equivalent to EMA Y
 
float EMA_a_low = 0.3;    //initialization of EMA alpha
float EMA_a_high = 0.5;
 
int EMA_S_low = 0;        //initialization of EMA S
int EMA_S_high = 0;
 
int highpass = 0; // filter out low frequencies 
int bandpass = 0; // fliter out high and low frequencies 

int timeServo=0; 
int timeDelay=0; 
 
void setup(){
  Serial.begin(115200);                   //setup of Serial module, 115200 bits/second
   
  EMA_S_low = analogRead(sensorPin);      //set EMA S for t=1
  EMA_S_high = analogRead(sensorPin);

  // SERVO VALUES
      pinMode(13,OUTPUT); // pin 13 is first servo 

        timeServo = 2000;     // INSERT TIME MOTOR SHOULD RUN TO OPEN HAND  IN MILLISECONDS 
        timeDelay = 5000;     // INSERT TIME IS TAKES BETWEEN SIGNALS TO RESET IN MILLISECONDS 
}
 
void loop(){
  sensorValue = analogRead(sensorPin);    //read the sensor value using ADC
   
  EMA_S_low = (EMA_a_low*sensorValue) + ((1-EMA_a_low)*EMA_S_low);  //run the EMA
  EMA_S_high = (EMA_a_high*sensorValue) + ((1-EMA_a_high)*EMA_S_high);
   
  //highpass = sensorValue - EMA_S_low;     //find the high-pass as before (for comparison)
  bandpass = EMA_S_high - EMA_S_low;      //find the band-pass  // for Dana +3 (turn on servo) // add timer between reading signals for recovery time 
 
  //Serial.print(highpass);
  //Serial.print(" ");
  Serial.println(bandpass);
   
  delay(20); 
  //20ms delay

  // CONTROLLING 1 SERVO, add 
      if(bandpass>=3) // change bandpass value for each person 
      digitalWrite(13, HIGH);
      delay(timeServo); 
      digitalWrite(13,LOW);
      delay(timeDelay);
}


