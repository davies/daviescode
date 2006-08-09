/*
   Delay class
   Delay buffer. Shifts input by speceified number of samples.
   
   Negative delay values means that stream is shifted backwards.

   'time_shift' - value of time shift should be applied at output (in samples!):
     negative number means that audio should be played before video.
     positive number means that audio should be played after video.
*/


#ifndef DELAY_H
#define DELAY_H

#include "defs.h"

class Delay
{
protected:
  sample_t*buf[NCHANNELS];
  int      beg[NCHANNELS];
  int      end[NCHANNELS];
  int      delay[NCHANNELS];
  bool     on;

public:
  int      time_shift;

  Delay();
  ~Delay();
  void reset();

  void process(sample_buffer_t &samples, Speakers spk);
  void get_delay(int *delay);
  void set_delay(int *delay);
  bool get_on()         { return on; }
  void set_on(bool _on);
};

#endif
