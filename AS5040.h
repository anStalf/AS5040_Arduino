/*
  Test.h - Test library for Wiring - description
  Copyright (c) 2006 John Doe.  All right reserved.
*/

// ensure this library description is only included once
#ifndef AS5040_h
#define AS5040_h

// include types & constants of Wiring core API
#include "Arduino.h"



class AS5040{
	public:
	AS5040(int CSn, int CLK, int DO);
	void exec(void);
	
	typedef struct _sens_data{
		int abs_angle;
		float angle;
		bool OCF;
		bool COF;
		bool LIN;
		bool magINC;
		bool magDEC;
	}sens_data;
	
	sens_data data;
	
	private:
	int CSpin;
	int CLKpin;
	int dCLKpin;
	int dDOpin;
	int DOpin;
	
	void wus(int us);
};


#endif

