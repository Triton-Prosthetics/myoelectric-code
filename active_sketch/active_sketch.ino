#include "arduinoFFT.h"

#define SAMPLES 128               // set to 32 point fft
#define SAMPLING_FREQUENCY 1000.0 // Hz; needs to be less than 10000

/* Setup FFT machinery: */
arduinoFFT FFT = arduinoFFT();
unsigned int sampling_period_us;
unsigned long microseconds;
double vReal[SAMPLES];
double vImag[SAMPLES];
double stuff = 1.00;


// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(115200);
  sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
}

// the loop routine runs over and over again forever:
void loop() {
  /*SAMPLING*/
  for(int i=0; i<SAMPLES; i++) {
    
    microseconds = micros();    //Overflows after around 70 minutes!
    //vReal[i] = analogRead(A0);
    vReal[i] = sin(2.0 * 3.14 * SAMPLING_FREQUENCY * (double) i) + sin(2.0 * 3.14 * 5.0 * SAMPLING_FREQUENCY * (double) i) + sin(2.0 * 3.14 * 6.0 * SAMPLING_FREQUENCY * (double) i);
    vImag[i] = 0;
     
    while(micros() < (microseconds + sampling_period_us)){
    }
  }
 
  /*FFT*/
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
  double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
 
    /*PRINT RESULTS*/
  //Serial.println(peak);     //Print out what frequency is the most dominant.
 
  for(int i=0; i<(SAMPLES/2); i++) {
    /*View all these three lines in serial terminal to see which frequencies has which amplitudes*/
    //Serial.print((i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES, 1);
    //Serial.print(" ");
    Serial.println(vReal[i], 1);    //View only this line in serial plotter to visualize the bins
  }
  
  stuff += 0.5;
  delay(1000);  //Repeat the process every second OR:
}
