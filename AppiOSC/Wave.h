/****************************************************/
/*  NOTE: the DAC used here has a resolution
          of 0 - 4095
*/
/***************************************************/

typedef struct {
  unsigned int peakMax = 4095;
  unsigned int peakMin = 0;
  unsigned int peak = 4095;
  byte peakRandom = 0; //none, both, top, bottom

  float freqMax = 20.0;
  float freqMin = 0.1;
  float freq = 1.0;
  float freqPos = 0.0;
  unsigned int arrayPos = 0;
  float freqStep = 0.0;
  byte freqRandom = 0; //no, yes

  byte waveType = 0; //sine, tri, sawU, sawD

  boolean lightOutput = true;
} Wave;

void makeDefaultWave (Wave* w, float frequency) {
  w->peakMax = 4095;
  w->peakMin = 0;
  w->peak = 4095;
  w->peakRandom = 0;
  w->freqMax = 20.0;
  w->freqMin = 0.1;
  w->freq = frequency;
  w->freqPos = 0.0;
  w->freqStep =  frequency / timerUpdate;
  w->freqRandom = 0;
  w->lightOutput = true;
}

void setFrequency(Wave* w, float frequency) {
  w->freq = constrain(frequency, w->freqMin, w->freqMax);
  w->freqStep = w->freq / timerUpdate;
}

void setAmplitude(Wave* w, float amplitude) {
  int newAmp = int(2047 * constrain(amplitude, 0.0, 1.0));
  w->peakMax = newAmp + 2048;
  w->peakMin = 2047 - newAmp;
}

void setWaveType(Wave* w, int type) {
  w->waveType = constrain(type, 0, 3);
}

void setWaveMinFreq(Wave* w, float minValue) {
  w->peakMin = constrain(int(minValue * 4095), 0, w->peakMax - 1);
}

void setWaveMaxFreq(Wave* w, float maxValue) {
  w->peakMax = constrain(int(maxValue * 4095), w->peakMin + 1, 4095);
}

void setWaveAmpRandom(Wave* w, int ampRand) {
  w->peakRandom = constrain(ampRand, 0, 3);
}

void setWaveFreqRandom(Wave* w, int newRand) {
  w->freqRandom = constrain(newRand, 0, 1);
}

void calcWaveValue(Wave* w) {

  if (w->freqPos >= 1.0) {  //  and if past cycle length   dutyPosition = 1.0 + delayValue
    w->freqPos -= 1.0;     //  then rollover position in cycle

    if (w->peakRandom == 1) {
      w->peakMax = random(w->peakMin, 4096);
      w->peakMin = random(0, w->peakMax);
    } else if (w->peakRandom == 2) {
      w->peakMax = random(w->peakMin, 4096);
    } else if (w->peakRandom == 3) {
      w->peakMin = random(0, w->peakMax);
    }

    if (w->freqRandom == 1) {
      w->freq = random(1, 200) / 10.0;
      w->freqStep = w->freq / timerUpdate;
    }
  }

  w->arrayPos = round(w->freqPos * timerUpdate);

  if (w->waveType == 0)
    w->peak = pgm_read_word(&Sine[w->arrayPos]);
  else if (w->waveType == 1)
    w->peak = pgm_read_word(&Tri[w->arrayPos]);
  else if (w->waveType == 2)
    w->peak = pgm_read_word(&SawU[w->arrayPos]);
  else
    w->peak = pgm_read_word(&SawD[w->arrayPos]);

  w->peak = map(w->peak, 0, 4095, w->peakMin, w->peakMax);

}



