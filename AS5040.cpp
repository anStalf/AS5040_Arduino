/*
  Test.h - Test library for Wiring - implementation
  Copyright (c) 2006 John Doe.  All right reserved.
*/



// include this library's description file
#include "AS5040.h"
//#define DEBUG
//Timing for AS5040 in microseconds
#define tstart 1000
#define tc 500
#define CS_l digitalWrite(this->CSpin, LOW)
#define CS_h digitalWrite(this->CSpin, HIGH)
#ifdef DEBUG
#define CLK0 digitalWrite(this->CLKpin, LOW); \
			 digitalWrite(this->dCLKpin, LOW); \
			 digitalWrite(this->dDOpin, digitalRead(this->DOpin))
#define CLK1 digitalWrite(this->CLKpin, HIGH); \
			 digitalWrite(this->dCLKpin, HIGH); \
			 digitalWrite(this->dDOpin, digitalRead(this->DOpin))
#endif
#ifndef DEBUG
#define CLK0 digitalWrite(this->CLKpin, LOW)
#define CLK1 digitalWrite(this->CLKpin, HIGH)
#endif

AS5040::AS5040(int CSn, int CLK, int DO){
	pinMode(CSn, OUTPUT);
	pinMode(CLK, OUTPUT);
	pinMode(DO, INPUT_PULLUP);
	this->CSpin = CSn;
	this->CLKpin = CLK;
	#ifdef DEBUG
	this->dCLKpin = 5;
	pinMode(this->dCLKpin, OUTPUT);
	this->dDOpin = 18;
	pinMode(this->dDOpin, OUTPUT);
	#endif
	this->DOpin = DO;
	CS_h;
	CLK1;
}

void AS5040::wus(int us){
	delayMicroseconds(us);
}


void AS5040::exec(void){
	bool recv_buffer[17];
	int temp = 0;
	CS_l; //Set to active state
	this->wus(tstart);
	CLK0;
	this->wus(tc);
	CLK1;
	for (int cnt = 0; cnt < 16; cnt++){
		recv_buffer[cnt] = digitalRead(this->DOpin);
		this->wus(tc);
		CLK0;
		this->wus(tc);
		CLK1;
	};
	CS_h;
	
	for (int cnt = 0; cnt < 11; cnt++){
		temp <<=1;
		temp += ((int)recv_buffer[cnt] & 0x01);
	}
	this->data.abs_angle = temp;
	if (temp == 0) temp = 1;
	this->data.angle = (float)temp/2.844444; //2.844~ Angle conversion constant from 1024 to 360
	this->data.OCF = (bool)recv_buffer[11]; //Offset Compensation Finished	
	this->data.COF = (bool)recv_buffer[12]; //Range error
	this->data.LIN = (bool)recv_buffer[13]; //Linear error
	this->data.magINC = (bool)recv_buffer[14]; //Magnet increase
	this->data.magDEC = (bool)recv_buffer[15]; //Magnet decrease
	
#ifdef DEBUG
	for (int cnt = 15; cnt > -1; cnt --){
		if (recv_buffer[cnt]){
			Serial.print("1");
		}else{
			Serial.print("0");
		}		
	}
	Serial.println (";");
#endif

}

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances



