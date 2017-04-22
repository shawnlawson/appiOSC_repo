/********************************************
Arduino project to read and write analog
Control Voltage to a modular synthesizer
from an Open Sound Control source.
********************************************/


//frequency in Hz to update ADC output
const int timerUpdate = 1000;

#include "WaveShapes.h"
#include "Wave.h"
#include "Wire.h"
//for addressing Adafruit ADC chips
#define TCAADDR 0x70
#define DACADDR 0x62
#define MCP4726_CMD_WRITEDAC 0x40

#include <OSCBundle.h>
#include <OSCBoards.h>
#include <SLIPEncodedSerial.h>
SLIPEncodedSerial SLIPSerial(Serial);


const byte numWaves = 4;
Wave waves[numWaves];
bool readAnalog = true;
const int analogIn[4] = {A0, A1, A2, A3};
float analogValue[4] = {0.0, 0.0, 0.0, 0.0};
const byte messageSendSleep = 10;
byte messageSleep = 0;

void setup() {
	SLIPSerial.begin(115200);
  	//  Serial.begin(9600);
	#if ARDUINO >= 100
	  while (!Serial)
	    ;   // Leonardo bug
	#endif


	//    Setup connections for multiplex DAC
	Wire.begin();
	Wire.setClock(400000L);
	tcaselect(0);

	for (int i = 0; i < 4; i++) {
		makeDefaultWave(&waves[i], 1.0);
	}

	setTimer();
	//  SLIPSerial.available();
}

void loop() {
	OSCMessage bundleIN;
	int size;

	while (!SLIPSerial.endofPacket() ) {
		if ((size = SLIPSerial.available()) > 0) {
			while (size--) {
				bundleIN.fill(SLIPSerial.read());
			}
		}

		for (int i = 0; i < 4; i++) {
			calcWaveValue(&waves[i]);
			tcaselect(i);
			setVoltage(waves[i].peak);
		}

		if (readAnalog && (messageSleep == messageSendSleep)) {
			for (int i = 0; i < 4; i++) {
		    	analogValue[i] = analogRead(analogIn[i]) / 1024.0;
			}

			OSCMessage msg("/analogInput");
			msg.add(analogValue[0]);
			msg.add(analogValue[1]);
			msg.add(analogValue[2]);
			msg.add(analogValue[3]);
			SLIPSerial.beginPacket();
			msg.send(SLIPSerial);
			SLIPSerial.endPacket();
			msg.empty();

			messageSleep = 0;
		}	// if(readAnalog)
		else {
			messageSleep++;
		}	// end if(readAnalog)
	}	//end while (!SLIPSerial.endofPacket() )


	if (!bundleIN.hasError()) {
		bundleIN.dispatch("/freq/set", freqSet);
		bundleIN.dispatch("/freq/amp", ampSet);
		bundleIN.dispatch("/freq/type", typeSet);
		bundleIN.dispatch("/freq/min", freqMinSet);
		bundleIN.dispatch("/freq/max", freqMaxSet);
		bundleIN.dispatch("/freq/ampRandom", ampRandomSet);
		bundleIN.dispatch("/freq/random", randomSet);
		bundleIN.dispatch("/freq/reset", resetSet);
	}
}


/**************************************************************************/
/*!
    @brief  OSC CALLBACKS
*/
/**************************************************************************/

void freqSet(OSCMessage & msg) {
	float newFreq = 1.0;
	if (msg.isFloat(0)) {
		newFreq = msg.getFloat(0);
	} else if (msg.isInt(1)) {
		newFreq = float(msg.getInt(0));
	}

	if (msg.isInt(1)) {
		byte whichLine = msg.getInt(1);
		if (whichLine < 0 || whichLine > 3)
		return;
	else
		setFrequency(&waves[whichLine], newFreq);
	}
}

void ampSet(OSCMessage & msg) {
	float newAmp = 1.0;
	if (msg.isFloat(0)) {
		newAmp = msg.getFloat(0);
	} else if (msg.isInt(1)) {
		newAmp = float(msg.getInt(0));
	}

	if (msg.isInt(1)) {
		byte whichLine = msg.getInt(1);
		if (whichLine < 0 || whichLine > 3)
			return;
		else {
			setAmplitude(&waves[whichLine], newAmp);
		}
	}
}

void typeSet(OSCMessage & msg) {
	int newType = 0;
	if (msg.isFloat(0)) {
		newType = int(msg.getFloat(0));
	} else if (msg.isInt(1)) {
		newType = msg.getInt(0);
	}

	if (msg.isInt(1)) {
		byte whichLine = msg.getInt(1);
		if (whichLine < 0 || whichLine > 3)
			return;
		else {
			setWaveType(&waves[whichLine], newType);
		}
	}
}

void freqMinSet(OSCMessage & msg) {
	float newMin = 0.0;
	if (msg.isFloat(0)) {
		newMin = msg.getFloat(0);
	} else if (msg.isInt(1)) {
		newMin = float(msg.getInt(0));
	}

	if (msg.isInt(1)) {
		byte whichLine = msg.getInt(1);
		if (whichLine < 0 || whichLine > 3)
			return;
		else {
			setWaveMinFreq(&waves[whichLine], newMin);
		}
	}
}

void freqMaxSet(OSCMessage & msg) {
	float newMax = 1.0;
	if (msg.isFloat(0)) {
		newMax = msg.getFloat(0);
	} else if (msg.isInt(1)) {
		newMax = float(msg.getInt(0));
	}

	if (msg.isInt(1)) {
		byte whichLine = msg.getInt(1);
		if (whichLine < 0 || whichLine > 3)
			return;
		else {
			setWaveMaxFreq(&waves[whichLine], newMax);
		}
	}
}

void ampRandomSet(OSCMessage & msg) {
	int newAmpRand = 0;
	if (msg.isFloat(0)) {
		newAmpRand = int(msg.getFloat(0));
	} else if (msg.isInt(1)) {
		newAmpRand = msg.getInt(0);
	}

	if (msg.isInt(1)) {
		byte whichLine = msg.getInt(1);
		if (whichLine < 0 || whichLine > 3)
			return;
		else {
			setWaveAmpRandom(&waves[whichLine], newAmpRand);
		}
	}
}

void randomSet(OSCMessage & msg) {
	int newRand = 0;
	if (msg.isFloat(0)) {
		newRand = int(msg.getFloat(0));
	} else if (msg.isInt(1)) {
		newRand = msg.getInt(0);
	}

	if (msg.isInt(1)) {
		byte whichLine = msg.getInt(1);
		if (whichLine < 0 || whichLine > 3)
			return;
		else {
			setWaveFreqRandom(&waves[whichLine], newRand);
		}
	}
}

void resetSet(OSCMessage & msg) {
	if (msg.isInt(1)) {
		byte whichLine = msg.getInt(1);
		if (whichLine < 0 || whichLine > 3)
			return;
		else {
			makeDefaultWave(&waves[whichLine], 1.0);
		}
	}
}

/**************************************************************************/
/*!
    @brief TIMER
*/
/**************************************************************************/

//	For Mega Arduino only, using a timer that doesn't collide with main
//	thread or with the pwm for two wire communication
void setTimer() {
	TCCR5A = 0;
	TCCR5B = 0;
	TCNT5 = 0;

	long cycles = (16000000L / (timerUpdate * 1024L)) - 1L; //timerUpdate in hz
	OCR5A = (int)cycles;
	TCCR5B = _BV(WGM52);
	TCCR5B |= _BV(CS52) | _BV(CS50);
	TIMSK5 = _BV(OCIE5B);
}

// can't do much more work than this interrupt the callback.
ISR(TIMER5_COMPB_vect) {
	waves[0].freqPos += waves[0].freqStep;
	waves[1].freqPos += waves[1].freqStep;
	waves[2].freqPos += waves[2].freqStep;
	waves[3].freqPos += waves[3].freqStep;
}


/**************************************************************************/
/*!
    @brief  WIRE HELPERS
*/
/**************************************************************************/

void tcaselect(uint8_t i) {
	if (i > 7) return;
	Wire.beginTransmission(TCAADDR);
	Wire.write(1 << i);
	Wire.endTransmission();
}

void setVoltage( uint16_t output) {
	Wire.beginTransmission(DACADDR);
	Wire.write(MCP4726_CMD_WRITEDAC);
	Wire.write(output >> 4);      
	// Upper data bits          (D11.D10.D9.D8.D7.D6.D5.D4)
	Wire.write((output & 15) << 4);
    // Lower data bits          (D3.D2.D1.D0.x.x.x.x)
	Wire.endTransmission();
}

