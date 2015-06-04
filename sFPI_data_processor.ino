/*  sFPI Processor version 1.0
    Copyright (c) 2015 David Miles Houston
    
    This sketch uses the following assoicated
    pins in conjunction with the MCP4725 DAC,
    for use with the output manipulation of the
    sFPI Driver program and with data collection,
    storage, and processing. It uses I2C to com-
    muicate with the DAC and its associated
    libraries. This data will be sent to a computer 
    via usb interface every cycle of the wave and 
    processed through an interactive GUI.
    
    Pin Name      Pin Location
    ---------    --------------
      SCL              A4
      SDA              A5
      Data_IN          A0
      Wave_IN          A1
      Signal_AT_1      A2
      Signal_AT_2      A3     
*/

#include <Wire.h>
#include "Adafruit_MCP4725.h"

Adafruit_MCP4725 dac;

#define bitres 1024;

int photodiode_data[bitres];
int counter = 0;
boolean transmit;


void pciSetup(byte pin)
{
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

void setup() {
  Serial.begin(9600);
  // Set A0, A1, A2 for analog inputs. 
  // A5 and A4 have already been set in the Adafruit Header File as SCL and SDA. 
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  
}
 
// Use one Routine to handle each group
 
ISR (PCINT0_vect) // handle pin change interrupt for D8 to D13 here
 {  
    if (counter < bitres) {
    photodiode_data[counter] = analogRead(A0);
    ramp_fucntion[counter] = analogRead(A1);
    counter++;
    }
    else {
      counter = 0;
      transmit = true;
    }
    
 } 

void loop(){
  
    if (counter < bitres) {
    photodiode_data[counter] = analogRead(A0);
    ramp_fucntion[counter] = analogRead(A1);
    counter++;
    }
    
    else {
      counter = 0;
      transmit = true;
    }
    
    if (transmit == true){
      Serial.write(photodiode_data);
      Serial.write(ramp_function);
    }  
}
  
    
    

