# AppiOSC

Is an arduino based device that translates Open Sound Control messages to a modular synthesizer. Created while on a Toolmaker Residency at [Signal Culture](http://signalculture.org). 

## Team

[Shawn Lawson](http://shawnlawson.com) 

[Ryan Ross Smith](http://ryanrosssmith.com)

[Frank Appio](https://github.com/fluxaxiom)

## OSC Appio

### to device

Message format is { message, float, float }. See Wave.h for more information. 

"/freq/set",  whatFrequency(freqMin - freqMax), whichOutput(0 - 4)

"/freq/amp",  whatAmplitude(0 - 1), whichOutput(0 - 4)

"/freq/type", whichWaveType(0 - 3), whichOutput(0 - 4)

"/freq/min", whatAmplitudeMinimum(0 - < peakMax), whichOutput(0 - 4)

"/freq/max", whatAmplitudeMaximum(> peakMin - 1), whichOutput(0 - 4)

"/freq/ampRandom", setRandomAmplitude(0 - 1), whichOutput(0 - 4)

"/freq/random", setRandomFrequncy(0 - 1), whichOutput(0 - 4)

"/freq/reset", setReset(0 - 1), whichOutput(0 - 4)

### from device

Message format is { message, float, float, float, float }. See AppiOSC.ino for more information. 

"/analogInput", inputLine0(0 - 1), inputLine1(0 - 1), inputLine2(0 - 1), inputLine3(0 - 1)

## Additionally helpful to know

- [AppiOSC](./AppiOSC) contains the arduino code, specifically for the Mega.
- [ArdunioLibraries](./ArdunioLibraries) contains the OSC library and specifically the SLIP headers for OSC over serial USB.
- [Images](./Images) contains images of the final device.
- [pythonGenerators](./pythonGenerators) contains python scripts for creating the wave tables.


## Sources

* http://signalculture.org
* http://www.liveinterfaces.org
* http://iclc.livecodenetwork.org/2016/
