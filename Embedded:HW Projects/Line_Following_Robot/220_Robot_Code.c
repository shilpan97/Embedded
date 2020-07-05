#include "mte220.c"  // C functions needed for labs

/*
;**********************************************************************
;
;    Filename:	    lab6.c
;    Date:          Nov 12/05
;    File Version:  3.1
;
;    Author:       Shilpan Shah
;
;    Copyright (c) C.C.W. Hulls, P.Eng and D.L. Wight, 2005.
;    This software can be used and modified by students, staff and
;    faculty members at the University of Waterloo for teaching or
;    research purposes, provided the authors are acknowledged.
;
;**********************************************************************
;
;    Files required:
;
;**********************************************************************
;
;    History:
;
;    3.1  Nov 12/05  use new functions and macros;
;    3.0  Nov  7/05  main code split into separate file that loads the
;                    functions needed for MTE 220
;
;    2.0  Nov  5/05  ported to C;
;
;    1.1  Oct 25/05  changed the phase of right servo to be midway
;                    through the entire cycle; enable peripheral interrupts
;                    as part of interrupt initialization
;    1.0  Oct 24/05  original release
;
;**********************************************************************
;**********************************************************************
;
;    Description:
;
;   The lab 6 code reads the IR sensors difference amplifier.  The IR
;   sensors are used for line following, so when a noticeable difference
;   is detected, the appropriate servo is turned off.
;
;**********************************************************************
*/

//;**********************************************************************

void speeddown(){

	SetLeft(3300);
	SetRight(3700);
	LongDelay(1);
	//SetLeft(3500);
	//SetRight(3320);
	BothServosOff;

}

void speedup(){

	SetLeft(3300);
	SetRight(3700);
	LongDelay(1);
	BothServosOn;

}

void hall_effect(){
	uns8 analog_value_halleffect; //current ADC value
	uns8 i;
	uns16 count1;
	uns16 count2; 
	count1 = 0;
	count2 = 0;
	analog_value_halleffect = AnalogConvert(ADC_HALL_EFFECT);

	while (AnalogConvert(ADC_HALL_EFFECT) < 0x5B){
		count1 += 1;
	}

	while (AnalogConvert(ADC_HALL_EFFECT) > 0xA8){
		count2 += 1;
	}

	if (count1 > 50){ // 1.8 Volts
		speeddown();
		for (i = 0; i<14; i++){  // 7 seconds ... Long Delay for 2 sec each... so 14 times iteration
			OnLED
				LongDelay(2);   // LongDelay takes in count. SO count x 0.125 = 0.25 ... total is 0.5..so 7 sec/ 0.5 = 14 iteations.
			OffLED
				LongDelay(2);
		}
		speedup();

	}
	else if (count2 > 50) { //3.3 Volts above threshold.. turn off servos.. turn on LED continuously

		speeddown();
		OnLED
			LongDelay(56); // LongDelay is for 7 seconds
		OffLED
			speedup();
	}
	
}


void main(void)
{
	uns8 analog_value;  // current ADC value
	uns8 analog_value_halleffect; //current ADC value
	uns8 i;
	Initialization();
	WaitForButton();
	OffLED
		
	// initially both servos are on
	UseServos         // (syntax of "call" is correct without () or ;)
	BothServosOn

	while (1)  // loop forever
	{
		analog_value = AnalogConvert(ADC_IR_SENSOR);  // get analog value from IR sensor diff amp // get analog value from Hall Effect Sensor
		 
		if (analog_value < 0x84) { // 0x66 is 1.38V for 10-bit ADC with 2 LSB dropped // 
			// left servo only
			LeftServoOn
			RightServoOff
			hall_effect();
		} else if (analog_value > 0x8E) { // 0x99 is 2.8V for 10-bit ADC with 2 LSB dropped //
			// right servo only
			RightServoOn
			LeftServoOff
			hall_effect();
		} else {
			// both servos on
			BothServosOn
			hall_effect();
		}	
	}
}

