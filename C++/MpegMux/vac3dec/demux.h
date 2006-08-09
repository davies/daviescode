#ifndef DEMUX_H
#define DEMUX_H
#include "defs.h"


#define DEMUX_SKIP   0
#define DEMUX_HEADER 1
#define DEMUX_DATA   2


//////////////////////////////////////////////////
// in-place MPEG1/2 PES demuxer
//
// PES-wrapped data in input buffer is replaced by
// unwrapped; length of unwrapped data is returned

class Demux
{
private:
  int state;
  uint8_t head_buf[268];
  int state_bytes;

  void move(uint8_t *data, int length);

public:
  int errors;

  Demux(): errors(0), state(DEMUX_SKIP), state_bytes(0) {};
  void reset() { state = DEMUX_SKIP; state_bytes = 0; };
  int decode(uint8_t *data, int length);
};


#endif
