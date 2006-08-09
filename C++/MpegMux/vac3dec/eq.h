#ifndef EQ_H
#define EQ_H

#include "defs.h"

class Eq
{
protected:
  sample_t func9[9];
  sample_t levels9[9];
  int      count9;

public:
  bool     on;

  Eq();
  void reset();
  void process(sample_buffer_t &samples, Speakers spk);
  void get_levels9(sample_t *levels);
  void set_func9(sample_t *func9);
  void get_func9(sample_t *func9);
};

#endif
