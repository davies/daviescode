#include <math.h>
#include "eq.h"


Eq::Eq()
{
  on = false;
  for (int i = 0; i < 9; i++)
    func9[i] = 1.0;

  reset();
}

void Eq::reset()
{
  memset(levels9, 0, sizeof(levels9));
  count9 = 0;
}

void Eq::process(sample_buffer_t &samples, Speakers spk)
{
  int ch, i, bin;
  sample_t factor;

  sample_t func[9];
  memcpy(func, func9, sizeof(func));

  // equalizer
  if (on)
  {
    for (ch = 0; ch < spk.nfchans(); ch++) 
    {
      levels9[0] += fabs(samples[ch][0] *= func[0]);
      levels9[1] += fabs(samples[ch][1] *= func[1]);
      bin = 2;
      factor = func[1];
      for (i = 2; i < 9; i++)
      {
        sample_t sum = 0;
        sample_t delta = (func[i] - func[i-1]) / ((1<<i) - (1<<(i-1)));
        for (; bin < (1<<i); bin++)
        {
          factor += delta;
          sum += fabs(samples[ch][bin] *= factor);
        }
        levels9[i] += sum / ((1<<i) - (1<<(i-1)));
      }
    }
    count9++;

    if (spk.lfe())
    {
      ch = spk.nfchans();
      sample_t sum = 0;
      levels9[0] += fabs(samples[ch][0] *= func9[0]);
      levels9[1] += fabs(samples[ch][1] *= func9[1]);

      sum = 0;
      sum += fabs(samples[ch][2] *= func9[2]);
      sum += fabs(samples[ch][3] *= func9[2]);
      levels9[2] += sum/2;

      sum = 0;
      sum += fabs(samples[ch][4] *= func9[3]);
      sum += fabs(samples[ch][5] *= func9[3]);
      sum += fabs(samples[ch][6] *= func9[3]);
      levels9[3] += sum/3;
    }
  }
}

void Eq::get_levels9(sample_t *levels)
{
  if (!count9)
    memset(levels, 0, sizeof(levels9));
  else
    for (int i = 0; i < 9; i++)
      levels[i] = levels9[i] / count9;

  memset(levels9, 0, sizeof(levels9));
  count9 = 0;
}

void Eq::get_func9(sample_t *_func)
{
  memcpy(_func, func9, sizeof(func9));
}

void Eq::set_func9(sample_t *_func)
{
  memcpy(func9, _func, sizeof(func9));
}
