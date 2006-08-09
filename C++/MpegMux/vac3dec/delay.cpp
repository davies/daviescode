#include <stdlib.h>
#include <string.h>
#include "delay.h"


Delay::Delay()
{
  memset(beg, 0, sizeof(beg));
  memset(end, 0, sizeof(beg));
  memset(buf, 0, sizeof(buf));
  memset(delay, 0, sizeof(buf));
  on = false;

  reset();
}

Delay::~Delay()
{
  for (int ch = 0; ch < NCHANNELS; ch++)
    if (buf[ch])
      delete buf[ch];
}

void Delay::reset()
{
  if (!on) return;
  for (int ch = 0; ch < NCHANNELS; ch++)
    if (beg[ch] > end[ch])
    {
      memset(buf[ch] + beg[ch], 0, sizeof(sample_t) * (MAX_DELAY - beg[ch]));
      memset(buf[ch], 0, sizeof(sample_t) * end[ch]);
    }
    else
      memset(buf[ch] + beg[ch], 0, sizeof(sample_t) * (end[ch] - beg[ch]));
}

void Delay::get_delay(int *_delay)
{
  memcpy(_delay, delay, sizeof(delay));
}

void Delay::set_delay(int *_delay)
{
  int ch;                               
  memcpy(&delay, _delay, sizeof(delay));

  time_shift = delay[0];
  for (ch = 1; ch < NCHANNELS; ch++)
    if (time_shift > delay[ch])
      time_shift = delay[ch];

  int delta[NCHANNELS];
  for (ch = 0; ch < NCHANNELS; ch++)
  {
    delta[ch] = delay[ch] - time_shift;
    if (delta[ch] > MAX_DELAY - NSAMPLES)
      delta[ch] = MAX_DELAY - NSAMPLES;
  }

  for (ch = 0; ch < NCHANNELS; ch++)
    if (delta[ch])
    {
      int e = beg[ch] + delta[ch] > MAX_DELAY? delta[ch] - (MAX_DELAY - beg[ch]): beg[ch] + delta[ch];
      int d = beg[ch] > end[ch]? MAX_DELAY - beg[ch] + end[ch]: end[ch] - beg[ch];

      // if we are not enabled buffers may be absent
      if (on && d < delta[ch])
        if (end[ch] > e)
        {
          memset(buf[ch] + end[ch], 0, sizeof(sample_t) * (MAX_DELAY - end[ch]));
          memset(buf[ch],           0, sizeof(sample_t) * e);
        }
        else
          memset(buf[ch] + end[ch], 0, sizeof(sample_t) * (e - end[ch]));

      end[ch] = e;
    }
    else
      beg[ch] = end[ch] = 0;
}

void Delay::process(sample_buffer_t &samples, Speakers spk)
{
  if (!on) return;

  for (int i = 0; i < NCHANNELS; i++) if (CH_MASK(spk.order[i]) & spk.channels)
  {
    int ch = spk.order[i];                  // channel number in std mapping (L C R SL SR LFE)
    int b = beg[ch];                        // begin pointer in delay buffer
    int e = end[ch];                        // end pointer in delay buffer
    sample_t *sb = samples[i];              // samples buffer begin
    sample_t *se = samples[i] + NSAMPLES;   // samples buffer end

    if (b != e)
    {
      int d = b > e? MAX_DELAY - b + e: e - b;
      int size = __min(NSAMPLES, d);

      if (e + size > MAX_DELAY)
      {
        int size1 = size - (MAX_DELAY - e);
        memcpy(buf[ch] + e, se - size,  sizeof(sample_t) * (size - size1));
        memcpy(buf[ch],     se - size1, sizeof(sample_t) * size1);
        end[ch] = size1;
      }
      else
      {
        memcpy(buf[ch] + e, se - size, sizeof(sample_t) * size);
        end[ch] += size;
      }

      if (d < NSAMPLES)
        memmove(sb + size, sb, sizeof(sample_t) * (NSAMPLES - size));

      if (b + size > MAX_DELAY)
      {
        int size1 = MAX_DELAY - b;
        memcpy(sb,         buf[ch] + b, sizeof(sample_t) * size1);
        memcpy(sb + size1, buf[ch],     sizeof(sample_t) * (size - size1));
        beg[ch] = size - size1;
      }
      else
      {
        memcpy(sb, buf[ch] + b, sizeof(sample_t) * size);
        beg[ch] += size;
      }
    }
  }
}


void Delay::set_on(bool _on)
{ 
  if (on == _on) return;
  on = _on;

  if (on)
  {
    int ch;

    // allocate buffer if needed
    for (ch = 0; ch < NCHANNELS; ch++)
      if (!buf[ch])
        buf[ch] = new sample_t[MAX_DELAY];

    // clear buffers
    for (ch = 0; ch < NCHANNELS; ch++)
      if (beg[ch] > end[ch])
      {
        memset(buf[ch] + beg[ch], 0, sizeof(sample_t) * (MAX_DELAY - beg[ch]));
        memset(buf[ch], 0, sizeof(sample_t) * end[ch]);
      }
      else
        memset(buf[ch] + beg[ch], 0, sizeof(sample_t) * (end[ch] - beg[ch]));
  }

  if (on)
    reset();
}
