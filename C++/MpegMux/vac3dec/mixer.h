/*
  Mixer

  Mixer and gain control class. Smooth gain control based on ac3 time window, 
  so delayed samples are also required for operation.

  Usage. 
    Create instance of a mixer, set input and output modes, set matrix 
    directly or call calc_matrix() function, then call mix() function.

    'level' - desired output level. It is guaranteed that Samples will not 
      exceed it.
    'clev', 'slev', 'lfelev' are params for matrix calculation calc_matrix().
    'master', 'gain', 'dynrng' are used in gain control and matrix-independent.
    'normalize' flag controls gain control behavior. True means one-pass
      normalization. So at at the beginning mixing use 'gain' = 'master'.
      When overflow occur gain is decreased and so on. When 'normalize' = false
      then after overflow gain begins to increase bit by bit until it
      reaches 'master' again or other overflow occur.
    'auto_gain' - automatic gain control. It will automatically lower gain 
      level on overload and restore it back then.
    'voice_control' - (only when stereo input) enables voice control when
      stereo input. Amplifies in-phase signal and route it to center 
      speaker if present. Only when auto_matrix = true.
    'expand_stereo' - (only when stereo input) enables surround control when
      stereo input. Amplifies out-of-phase signal and route it to surround 
      speakers if present. Only when auto_matrix = true.



    calc_matrix() - calc mixing matrix complied with ac3 standart (not normalized)
    normalize() - normalizes matrix so no overflow at output if no overflow at input.
    reset() - reset time window, reset to 'master' to 'gain' and 'dynrng' to 1.0.
*/

#ifndef MIXER_H
#define MIXER_H

#include "defs.h"

class Mixer
{
protected:
  sample_buffer_t delay;

public:
  Speakers in_mode;
  Speakers out_mode;

  sample_t in_levels[NCHANNELS];
  sample_t out_levels[NCHANNELS];
  sample_t clev;               // central mix level
  sample_t slev;               // surround mix level
  sample_t lfelev;             // lfe mix level

  sample_t level;              // desired output level
  sample_t dynrng;             // dynamic range compression
  sample_t master;             // desired gain (manually set)
  sample_t gain;               // current gain              
  bool     auto_gain;          // auto gain contol
  bool     normalize;          // one-pass normalize
  bool     expand_stereo;      // controls out-of-phase signal components
  bool     voice_control;      // controls in-phase signal components

  mixer_matrix_t matrix;

  Mixer();
  void reset();
  void calc_matrix(bool normalize = false);
  void normalize_matrix();
  void mix(sample_buffer_t &samples, sample_buffer_t &new_delay);
};

#endif
