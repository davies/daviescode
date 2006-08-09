#include <math.h>
#include <memory.h>
#include "mixer.h"

#define value2db(value) ((value > 0)? log10(value)*20.0: 0)
#define db2value(db)    pow(10.0, db/20.0)

sample_t min_gain = 0.1;

// time window
const sample_t window[] = 
{
  0.00014, 0.00024, 0.00037, 0.00051, 0.00067, 0.00086, 0.00107, 0.00130,
  0.00157, 0.00187, 0.00220, 0.00256, 0.00297, 0.00341, 0.00390, 0.00443,
  0.00501, 0.00564, 0.00632, 0.00706, 0.00785, 0.00871, 0.00962, 0.01061,
  0.01166, 0.01279, 0.01399, 0.01526, 0.01662, 0.01806, 0.01959, 0.02121,
  0.02292, 0.02472, 0.02662, 0.02863, 0.03073, 0.03294, 0.03527, 0.03770,
  0.04025, 0.04292, 0.04571, 0.04862, 0.05165, 0.05481, 0.05810, 0.06153,
  0.06508, 0.06878, 0.07261, 0.07658, 0.08069, 0.08495, 0.08935, 0.09389,
  0.09859, 0.10343, 0.10842, 0.11356, 0.11885, 0.12429, 0.12988, 0.13563,
  0.14152, 0.14757, 0.15376, 0.16011, 0.16661, 0.17325, 0.18005, 0.18699,
  0.19407, 0.20130, 0.20867, 0.21618, 0.22382, 0.23161, 0.23952, 0.24757,
  0.25574, 0.26404, 0.27246, 0.28100, 0.28965, 0.29841, 0.30729, 0.31626,
  0.32533, 0.33450, 0.34376, 0.35311, 0.36253, 0.37204, 0.38161, 0.39126,
  0.40096, 0.41072, 0.42054, 0.43040, 0.44030, 0.45023, 0.46020, 0.47019,
  0.48020, 0.49022, 0.50025, 0.51028, 0.52031, 0.53033, 0.54033, 0.55031,
  0.56026, 0.57019, 0.58007, 0.58991, 0.59970, 0.60944, 0.61912, 0.62873,
  0.63827, 0.64774, 0.65713, 0.66643, 0.67564, 0.68476, 0.69377, 0.70269,
  0.71150, 0.72019, 0.72877, 0.73723, 0.74557, 0.75378, 0.76186, 0.76981,
  0.77762, 0.78530, 0.79283, 0.80022, 0.80747, 0.81457, 0.82151, 0.82831,
  0.83496, 0.84145, 0.84779, 0.85398, 0.86001, 0.86588, 0.87160, 0.87716,
  0.88257, 0.88782, 0.89291, 0.89785, 0.90264, 0.90728, 0.91176, 0.91610,
  0.92028, 0.92432, 0.92822, 0.93197, 0.93558, 0.93906, 0.94240, 0.94560,
  0.94867, 0.95162, 0.95444, 0.95713, 0.95971, 0.96217, 0.96451, 0.96674,
  0.96887, 0.97089, 0.97281, 0.97463, 0.97635, 0.97799, 0.97953, 0.98099,
  0.98236, 0.98366, 0.98488, 0.98602, 0.98710, 0.98811, 0.98905, 0.98994,
  0.99076, 0.99153, 0.99225, 0.99291, 0.99353, 0.99411, 0.99464, 0.99513,
  0.99558, 0.99600, 0.99639, 0.99674, 0.99706, 0.99736, 0.99763, 0.99788,
  0.99811, 0.99831, 0.99850, 0.99867, 0.99882, 0.99895, 0.99908, 0.99919,
  0.99929, 0.99938, 0.99946, 0.99953, 0.99959, 0.99965, 0.99969, 0.99974,
  0.99978, 0.99981, 0.99984, 0.99986, 0.99988, 0.99990, 0.99992, 0.99993,
  0.99994, 0.99995, 0.99996, 0.99997, 0.99998, 0.99998, 0.99998, 0.99999,
  0.99999, 0.99999, 0.99999, 1.00000, 1.00000, 1.00000, 1.00000, 1.00000,
  1.00000, 1.00000, 1.00000, 1.00000, 1.00000, 1.00000, 1.00000, 1.00000
};





Mixer::Mixer()
{
  for (int i = 0; i < NCHANNELS; i++)
    for (int j = 0; j < NCHANNELS; j++)
      matrix[i][j] = 0;

  slev   = 1.0;
  clev   = 1.0;
  lfelev = 1.0;

  level  = 1.0;
  master = 1.0;
  gain   = 1.0;
  dynrng = 1.0;
  auto_gain = true;
  normalize = false;
  expand_stereo = true;
  voice_control = true;

  memset(&in_levels, 0, sizeof(in_levels));
  memset(&out_levels, 0, sizeof(out_levels));
  memset(&delay, 1, sizeof(delay));

  in_mode  = Speakers(MODE_STEREO);
  out_mode = Speakers(MODE_STEREO);
}

void 
Mixer::reset()
{
  memset(&in_levels, 0, sizeof(in_levels));
  memset(&out_levels, 0, sizeof(out_levels));
  memset(&delay, 0, sizeof(delay));
  gain = master;
  dynrng = 1.0;
}

void 
Mixer::mix(sample_buffer_t &samples, sample_buffer_t &new_delay)
{
  sample_t buf[NCHANNELS];
  int ch, nsample;

  int in_ch   = in_mode.channels;
  int out_ch  = out_mode.channels;
  int in_nch  = in_mode.nchans();
  int out_nch = out_mode.nchans();
  int in_order[6];
  int out_order[6];
  memcpy(in_order, in_mode.order, sizeof(in_order));
  memcpy(out_order, out_mode.order, sizeof(out_order));

  sample_t max;
  sample_t factor;
  sample_t w;
  sample_t s;

  ///////////////////////////////////////
  // Reorder matrix

  mixer_matrix_t m;
  memset(&m, 0, sizeof(m));
  int ch1, ch2;
  for (ch1 = 0; ch1 < in_nch; ch1++)
    for (ch2 = 0; ch2 < out_nch; ch2++)
      m[ch1][ch2] = matrix[in_order[ch1]][out_order[ch2]];

  ///////////////////////////////////////
  // Input levels

  // optimize: expand channels cycle
  memset(in_levels, 0, sizeof(in_levels));
  for (ch = 0; ch < NCHANNELS; ch++)
    if (CH_MASK(in_order[ch]) & in_ch)
    {
      max = in_levels[in_order[ch]];
      for (nsample = 0; nsample < NSAMPLES; nsample++)
        if (fabs(samples[ch][nsample]) > max) 
          max = fabs(samples[ch][nsample]);
      in_levels[in_order[ch]] = max;
    }

  ///////////////////////////////////////
  // Adjust gain

  if (auto_gain && !normalize)
  {
    if (fabs(gain/master - 1) > 0.0003 )
    {
      if (gain < master)
        gain *= 1.0002;
      else
        gain /= 1.0002;
    }
    else
      gain = master;
  }

  ///////////////////////////////////////
  // Mix samples

  // optimize: do not multiply by nulls (most of matrix)
  factor = level * gain * dynrng;
  memset(buf, 0, sizeof(buf));
  for (nsample = 0; nsample < NSAMPLES; nsample++)
  {
    w = window[nsample] * factor;
    for (ch = 0; ch < out_nch; ch++)
    {
      buf[ch] = 0;
      buf[ch] += samples[0][nsample] * m[0][ch];
      buf[ch] += samples[1][nsample] * m[1][ch];
      buf[ch] += samples[2][nsample] * m[2][ch];
      buf[ch] += samples[3][nsample] * m[3][ch];
      buf[ch] += samples[4][nsample] * m[4][ch];
      buf[ch] += samples[5][nsample] * m[5][ch];
    }
    samples[0][nsample] = buf[0] * w;
    samples[1][nsample] = buf[1] * w;
    samples[2][nsample] = buf[2] * w;
    samples[3][nsample] = buf[3] * w;
    samples[4][nsample] = buf[4] * w;
    samples[5][nsample] = buf[5] * w;
  }

  ///////////////////////////////////////
  // Auto Gain Control

  if (auto_gain)
  {
    max = 0;
    for (ch = 0; ch < NCHANNELS; ch++)
      if (CH_MASK(out_order[ch]) & out_ch)
        for (nsample = 0; nsample < NSAMPLES; nsample++)
        {
          s = fabs(samples[ch][nsample]);
          if (max < s) max = s;
        }

    if (max > level * LEVEL_3DB)
    {
      factor = level * LEVEL_3DB / max;
      gain *= factor;
      if (gain < min_gain)
        gain = min_gain;

      for (nsample = 0; nsample < NSAMPLES; nsample++)
      {
        samples[0][nsample] *= factor;
        samples[1][nsample] *= factor;
        samples[2][nsample] *= factor;
        samples[3][nsample] *= factor;
        samples[4][nsample] *= factor;
        samples[5][nsample] *= factor;
      }
    }
  }

  ///////////////////////////////////////
  // Add delay and clip

  for (ch = 0; ch < NCHANNELS; ch++)
    if (CH_MASK(out_order[ch]) & out_ch)
      for (nsample = 0; nsample < 256; nsample++)
      {
        s = samples[ch][nsample] += delay[ch][nsample];
        if (s >  level) samples[ch][nsample] = level;
        if (s < -level) samples[ch][nsample] = -level;
      }

  ///////////////////////////////////////
  // Output levels

  // optimize: expand channels cycle
  memset(out_levels, 0, sizeof(out_levels));
  for (ch = 0; ch < NCHANNELS; ch++)
    if (CH_MASK(out_order[ch]) & out_ch)
    {
      max = out_levels[out_order[ch]];
      for (nsample = 0; nsample < NSAMPLES; nsample++)
        if (fabs(samples[ch][nsample]) > max) 
          max = fabs(samples[ch][nsample]);
      out_levels[out_order[ch]] = max;
    }

  ///////////////////////////////////////
  // Mix new delay

  // optimize: do not multiply by nulls (most of matrix)
  memset(buf, 0, sizeof(buf));
  factor = level * gain * dynrng;
  for (nsample = 0; nsample < NSAMPLES; nsample++)
  {
    w = factor * window[255 - nsample];
    for (ch = 0; ch < out_nch; ch++)
    {
      buf[ch] = 0;
      buf[ch] += new_delay[0][nsample] * m[0][ch];
      buf[ch] += new_delay[1][nsample] * m[1][ch];
      buf[ch] += new_delay[2][nsample] * m[2][ch];
      buf[ch] += new_delay[3][nsample] * m[3][ch];
      buf[ch] += new_delay[4][nsample] * m[4][ch];
      buf[ch] += new_delay[5][nsample] * m[5][ch];
    }
    delay[0][nsample] = buf[0] * w;
    delay[1][nsample] = buf[1] * w;
    delay[2][nsample] = buf[2] * w;
    delay[3][nsample] = buf[3] * w;
    delay[4][nsample] = buf[4] * w;
    delay[5][nsample] = buf[5] * w;
  }
}

void 
Mixer::calc_matrix(bool normalize)
{
  int in_ch      = in_mode.channels;
  int out_ch     = out_mode.channels;
  int in_nfront  = in_mode.nfront();
  int in_nrear   = in_mode.nrear();
  int out_nfront = out_mode.nfront();
  int out_nrear  = out_mode.nrear();

  memset(&matrix, 0, sizeof(mixer_matrix_t));



  // Dolby modes are backwards-compatible
  if (in_mode.dolby & out_mode.dolby)
  {
    matrix[CH_L][CH_L] = 1;
    matrix[CH_R][CH_R] = 1;
  }
  // Downmix to Dolby Surround/ProLogic/ProLogicII
  else if (out_mode.dolby)
  {
    if (in_nfront >= 2)
    {
      matrix[CH_L][CH_L] = 1;
      matrix[CH_R][CH_R] = 1;
    }

    if (in_nfront != 2)
    {
      matrix[CH_C][CH_L] = 0.7071 * clev;
      matrix[CH_C][CH_R] = 0.7071 * clev;
    }
    if (in_nrear == 1)
    {
      matrix[CH_S][CH_L] = -0.7071 * slev;
      matrix[CH_S][CH_R] = +0.7071 * slev;
    }
    else if (in_nrear == 2)
    {
      switch (out_mode.dolby)
      { 
      case DOLBY_PROLOGICII:
        matrix[CH_SL][CH_L] = -0.8660*slev;
        matrix[CH_SR][CH_L] = -0.5000*slev;
        matrix[CH_SL][CH_R] = +0.5000*slev;
        matrix[CH_SR][CH_R] = +0.8660*slev;
        break;

      case DOLBY_SURROUND:
      case DOLBY_PROLOGIC:
      default:
        matrix[CH_SL][CH_L] = -slev;
        matrix[CH_SR][CH_L] = -slev;
        matrix[CH_SL][CH_R] = +slev;
        matrix[CH_SR][CH_R] = +slev;
        break;
      }
    }
  }
  else
  // A/52 standart mixes
  {
    // direct route equal channels
    if (in_ch & out_ch & CH_MASK_L)   matrix[CH_L]  [CH_L]   = 1.0;
    if (in_ch & out_ch & CH_MASK_R)   matrix[CH_R]  [CH_R]   = 1.0;
    if (in_ch & out_ch & CH_MASK_C)   matrix[CH_C]  [CH_C]   = clev;
    if (in_ch & out_ch & CH_MASK_SL)  matrix[CH_SL] [CH_SL]  = slev;
    if (in_ch & out_ch & CH_MASK_SR)  matrix[CH_SR] [CH_SR]  = slev;

    // calc matrix for fbw channels
    if (out_nfront == 1)
    {
      if (in_nfront != 1)
      {
        matrix[CH_L][CH_M] = LEVEL_3DB;
        matrix[CH_R][CH_M] = LEVEL_3DB;
      }
      if (in_nfront == 3)
      {
        matrix[CH_C][CH_M] = clev * LEVEL_PLUS3DB;
      }
      if (in_nrear == 1)
      {
        matrix[CH_S][CH_M] = slev * LEVEL_3DB;
      }
      else
      {
        matrix[CH_SL][CH_M] = slev * LEVEL_3DB;
        matrix[CH_SR][CH_M] = slev * LEVEL_3DB;
      }
    }
    else // not mono modes
    {
      if (out_nfront == 2)
      {
        if (in_nfront == 1)
        {
          matrix[CH_M][CH_L] = LEVEL_3DB;
          matrix[CH_M][CH_R] = LEVEL_3DB;
        }
        else if (in_nfront == 3)
        {
          matrix[CH_C][CH_L] = clev;
          matrix[CH_C][CH_R] = clev;
        }
      }
      if (in_nrear == 1)
      {
        if (out_nrear == 0)
        {
          matrix[CH_S][CH_L] = slev * LEVEL_3DB;
          matrix[CH_S][CH_R] = slev * LEVEL_3DB;
        }
        else if (out_nrear == 2)
        {
          matrix[CH_S][CH_SL] = slev * LEVEL_3DB;
          matrix[CH_S][CH_SR] = slev * LEVEL_3DB;
        }
      }
      else if (in_nrear == 2)
      {
        if (out_nrear == 0)
        {
          matrix[CH_SL][CH_L] = slev;
          matrix[CH_SR][CH_R] = slev;
        }         
        else if (out_nrear == 1)
        {
          matrix[CH_SL][CH_S] = slev * LEVEL_3DB;
          matrix[CH_SR][CH_S] = slev * LEVEL_3DB;
        }
      }
    }
  }

  // Expand stereo & Voice control
  bool expand_stereo_allowed = expand_stereo && !in_nrear;
  bool voice_control_allowed = voice_control && (in_nfront == 2);

  if ((voice_control_allowed || expand_stereo_allowed) && !out_mode.dolby)
  {
    if (voice_control_allowed && out_nfront != 2)
    {
      // C' = clev * (L + R) * LEVEL_3DB
      matrix[CH_L][CH_M] = clev * LEVEL_3DB;
      matrix[CH_R][CH_M] = clev * LEVEL_3DB;
    }

    if (expand_stereo_allowed && in_nfront == 2 && out_nrear)
    {
      if (out_nrear == 1)
      {
        // S' = slev * (L - R)
        matrix[CH_L][CH_S] = + slev;
        matrix[CH_R][CH_S] = - slev;
      }
      if (out_nrear == 2)
      {
        // SL' = slev * 1/2 (L - R)
        // SR' = slev * 1/2 (R - L)
        matrix[CH_L][CH_SL] = + 0.5 * slev;
        matrix[CH_R][CH_SL] = - 0.5 * slev;
        matrix[CH_L][CH_SR] = - 0.5 * slev;
        matrix[CH_R][CH_SR] = + 0.5 * slev;
      }
    }

    if (in_nfront != 1)
    {
      if (expand_stereo_allowed && voice_control_allowed)
      {
        // L' = L * 1/2 (slev + clev) - R * 1/2 (slev - clev)
        // R' = R * 1/2 (slev + clev) - L * 1/2 (slev - clev)
        matrix[CH_L][CH_L] = + 0.5 * (slev + clev);
        matrix[CH_R][CH_L] = - 0.5 * (slev - clev);
        matrix[CH_L][CH_R] = - 0.5 * (slev - clev);
        matrix[CH_R][CH_R] = + 0.5 * (slev + clev);
      }
      else if (expand_stereo_allowed)
      {
        matrix[CH_L][CH_L] = + 0.5 * (slev + 1);
        matrix[CH_R][CH_L] = - 0.5 * (slev - 1);
        matrix[CH_L][CH_R] = - 0.5 * (slev - 1);
        matrix[CH_R][CH_R] = + 0.5 * (slev + 1);
      }
      else // if (voice_control_allowed)
      {
        matrix[CH_L][CH_L] = + 0.5 * (1 + clev);
        matrix[CH_R][CH_L] = - 0.5 * (1 - clev);
        matrix[CH_L][CH_R] = - 0.5 * (1 - clev);
        matrix[CH_R][CH_R] = + 0.5 * (1 + clev);
      }
    }
  }

  // LFE mixing
  if (in_ch & out_ch & CH_MASK_LFE) 
    matrix[CH_LFE][CH_LFE] = lfelev;

  // mix LFE into front channels if exists in input
  // and absent in output
  if (in_ch & ~out_ch & CH_MASK_LFE)
  {
    if (out_nfront > 1)
    {
      matrix[CH_LFE][CH_L]  = lfelev;
      matrix[CH_LFE][CH_R]  = lfelev;
    }
    else
      matrix[CH_LFE][CH_C]  = lfelev;
  }

  if (normalize)
    normalize_matrix();
}

void 
Mixer::normalize_matrix()
{
  sample_t levels[NCHANNELS] = { 0, 0, 0, 0, 0, 0 };
  sample_t max_level;
  sample_t norm;
  int i, j;

  for (i = 0; i < NCHANNELS; i++)
    for (j = 0; j < NCHANNELS; j++)
      levels[i] += matrix[j][i];

  max_level = levels[0];
  for (i = 1; i < NCHANNELS; i++)
    if (levels[i] > max_level) 
      max_level = levels[i];

  if (max_level > 0)
    norm = 1.0/max_level;
  else
    norm = 1.0;

  for (i = 0; i < NCHANNELS; i++)
    for (j = 0; j < NCHANNELS; j++)
      matrix[j][i] *= norm;
}
