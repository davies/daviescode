#include <string.h>
#include "decoder.h"


PushDecoder::PushDecoder(Speakers spk)
{
  memset(&bsi, 0, sizeof(bsi));
  frames = 0;
  errors = 0;

  is_pes            = false;

  dynrng            = true;
  dynrng_power      = 1.0;

  slev_lock         = true;
  clev_lock         = true;
  lfelev_lock       = true;

  auto_matrix       = true;
  normalize_matrix  = true;
  bass_redir        = true;

  set_mode(spk);
}

void 
PushDecoder::reset()
{
  bufptr = buf;
  bufpos = buf + 7;
//  memset(&bsi, 0, sizeof(bsi));
  memset(&delay, 0, sizeof(delay));
  memset(&samples, 0, sizeof(samples));

  parser.reset();
  demux.reset();
  mixer.reset();
  eq.reset();
  d.reset();
}

void 
PushDecoder::decode(uint8_t *buffer, uint32_t length)
{
  if (is_pes)
    length = demux.decode(buffer, length);

  long len;

  // while we have something in buffer we will try to decode it
  while (1) 
  {
    // copy requested data to buf from buffer if we have requested amount
    // or copy rest of buffer to remember it for future calls so we have processed all given data

    len = length;

    if (!len) break;

    if (len > bufpos - bufptr)
      len = bufpos - bufptr;

    memcpy (bufptr, buffer, len);
    bufptr += len;
    buffer += len;
    length -= len;

    // if we have all requested data
    if (bufptr == bufpos) 
    {
      // find sync, decode frame header and load frame data
      if (bufpos == buf + 7) 
      {
        int frame_size = parser.syncinfo (buf);

        // skip if it is not a sync and cycle
        if (!frame_size) 
        {
          for (bufptr = buf; bufptr < buf + 6; bufptr++)
             bufptr[0] = bufptr[1];
          continue;
        }

        // or request more data
        bufpos = buf + frame_size;
        sync();
      } 
      // decode frame data
      else 
      {
        if (!parser.frame (buf))
        {
          parser.get_bsi(bsi);
          frame();
          frames++;
        }        
        else
          errors++;
        bufptr = buf;
        bufpos = buf + 7;
      }
    }
  }
}

void 
PushDecoder::frame()
{
  Speakers spk = bsi.spk;

  // Create virtual LFE channel for bass redirection
  // if it is subwoofer at output and no lfe at input
  if (bass_redir & !spk.lfe() & mixer.out_mode.lfe())
  {
    spk.mode |= MODE_LFE;
    spk.channels |= CH_MASK_LFE;
    spk.order[spk.nfchans()] = CH_LFE;
  }

  // Set frame-level mixing params
  mixer.in_mode = spk;

  if (clev_lock)
    // Use parser 'clev' value only if we're making center channel downmix
    if (mixer.out_mode.dolby || (mixer.out_mode.channels & CH_MASK_C))
      mixer.clev = 1.0;
    else
      mixer.clev = parser.clev;

  if (slev_lock)   
    // Use parser 'slev' value only if we're making surround channels downmix
    if (mixer.out_mode.dolby || (mixer.out_mode.channels & (CH_MASK_SR | CH_MASK_SR)))
      mixer.slev = 1.0;
    else
      mixer.slev = parser.slev;

  if (lfelev_lock) mixer.lfelev = 1.0;
  if (!dynrng)     mixer.dynrng = 1.0;

  if (auto_matrix)
    mixer.calc_matrix(normalize_matrix);

  for (int i = 0; i < 6; i++) 
  {
    // perfomance warinig:
    // after previous mixing all channels may 
    // be changed so clear all?
    // fixes bug 07.07.02 on Dracula film
    memset(samples, 0, sizeof(samples));
    memset(delay, 0, sizeof(delay));

    if (parser.parse_block(samples))
    {
      errors++;
      break;
    }

    parser.get_bsi(bsi);

    // pre-processing in frequency domain
    // equalizer, filtering, bass management
    process_coefs();

    // IMDCT
    for (int ch = 0; ch < spk.nfchans(); ch++) 
      if (parser.blksw[ch])
        imdct.imdct_256(samples[ch], delay[ch]);
      else 
        imdct.imdct_512(samples[ch], delay[ch]);

    if (spk.lfe())
      imdct.imdct_512(samples[spk.nfchans()], delay[spk.nfchans()]);

    // post-processing in time domain
    // mixing by default.
    process_samples();

    // do something useful
    block();
  }
}

void
PushDecoder::process_coefs()
{
  Speakers in_spk  = mixer.in_mode;
  Speakers out_spk = mixer.out_mode;

  // 100-7000Hz filter for surround channels for DolbySurround Downmix
  if (!in_spk.dolby &&  // do not allow dolby-to-dolby mixes
      (out_spk.dolby == DOLBY_SURROUND) || (out_spk.dolby == DOLBY_PROLOGIC))
  {
    int high = 7000 * 512 / bsi.sample_rate; 
    // first surround channel
    if (in_spk.acmod() >= 4)
    {
      samples[CH_SL][0] = 0;
      for (int i = high; i < 256; i++)
        samples[CH_SL][i] = 0;
    }
    // second surround channel
    if (in_spk.acmod() >= 6)
    {
      samples[CH_SR][0] = 0;
      for (int i = high; i < 256; i++)
        samples[CH_SR][i] = 0;
    }
  }

  // Bass redirection
  if (bass_redir && out_spk.lfe())
  {
    int lfe_ch = in_spk.nfchans();
    for (int ch = 0; ch < in_spk.nfchans(); ch++) 
    {
      samples[lfe_ch][0] += samples[ch][0];
      samples[lfe_ch][1] += samples[ch][1];
      samples[lfe_ch][2] += samples[ch][2];
      samples[lfe_ch][3] += samples[ch][3];
    }
  }

  eq.process(samples, bsi.spk);
}

void
PushDecoder::process_samples()
{
  // mixer
  if (dynrng)
    mixer.dynrng = pow(parser.dynrng, dynrng_power);
  mixer.mix(samples, delay);
  d.process(samples, mixer.out_mode);
}


///////////////////////////////////////////////////////////////////////////////


PullDecoder::PullDecoder(Speakers spk)
{
  memset(&bsi, 0, sizeof(bsi));
  frames = 0;
  errors = 0;

  is_pes           = false;

  dynrng           = true;
  dynrng_power     = 1.0;

  slev_lock        = true;
  clev_lock        = true;
  lfelev_lock      = true;

  auto_matrix      = true;
  normalize_matrix = true;

  for (int nsample = 0; nsample < 256; nsample++)
    equalizer[nsample] = 1.0;

  set_mode(spk);
}

void 
PullDecoder::reset()
{
  blocks = 0;
//  memset(&bsi, 0, sizeof(bsi));
  memset(&delay, 0, sizeof(delay));
  memset(&samples, 0, sizeof(samples));

  parser.reset();
  demux.reset();
  mixer.reset();
  d.reset();
}

bool 
PullDecoder::frame(int max_sync_scan)
{
  long len;

  uint8_t *bufptr = buf;    
  uint8_t *bufpos = buf + 7;

  while (len = get(bufptr, bufpos - bufptr))
  {
    if (max_sync_scan < len)
      return false;
    else
      max_sync_scan -= len;

    if (is_pes)
      len = demux.decode(bufptr, len);

    bufptr += len;
    if (bufptr < bufpos)
      continue;

    // find sync, decode frame header and load frame data
    if (bufpos == buf + 7) 
    {
      int frame_size = parser.syncinfo (buf);

      // skip if it is not a sync and cycle
      if (!frame_size) 
      {
        for (bufptr = buf; bufptr < buf + 6; bufptr++)
           bufptr[0] = bufptr[1];
        continue;
      }

      // or request more data
      bufpos = buf + frame_size;
    } 
    // decode frame data
    else 
    {
      if (!parser.frame (buf))
      {
        parser.get_bsi(bsi);
        blocks = 6;
        frames++;

        Speakers spk = bsi.spk;

        // Set frame-level mixing params
        mixer.in_mode = spk;
        if (clev_lock)   mixer.clev = (mixer.out_mode.nfront() != 2)? 1.0: parser.clev;
        if (slev_lock)   mixer.slev = mixer.out_mode.surround()? 1.0: parser.slev;
        if (lfelev_lock) mixer.lfelev = 1.0;
        if (!dynrng)     mixer.dynrng = 1.0;

        if (auto_matrix)
          mixer.calc_matrix(normalize_matrix);

        return true;
      }        
      else
        errors++;

      bufptr = buf;
      bufpos = buf + 7;
    }
  }
  return false;
}

bool 
PullDecoder::block()
{
  if (!blocks)
    if (!frame())
      return false;

  // perfomance warinig:
  // after previous mixing all channels may 
  // be changed so clear all?
  // fixes bug 07.07.02 on Dracula film
  memset(samples, 0, sizeof(samples));
  memset(delay, 0, sizeof(delay));

  if (parser.parse_block(samples))
  {
    errors++;
    blocks = 0;                                         
    return false;
  }

  parser.get_bsi(bsi);
  Speakers spk = bsi.spk;

  // pre-processing in frequency domain
  // equalizer by default
  process_coefs();

  // IMDCT
  for (int ch = 0; ch < spk.nfchans(); ch++) 
    if (parser.blksw[ch])
      imdct.imdct_256(samples[ch], delay[ch]);
    else 
      imdct.imdct_512(samples[ch], delay[ch]);

  if (spk.lfe())
    imdct.imdct_512(samples[spk.nfchans()], delay[spk.nfchans()]);

  // post-processing in time domain
  // mixing by default.
  process_samples();

  blocks--;
  return true;
}


void
PullDecoder::process_coefs()
{
  Speakers spk = bsi.spk;
  eq.process(samples, bsi.spk);
}

void
PullDecoder::process_samples()
{
  // mixer
  if (dynrng)
    mixer.dynrng = pow(parser.dynrng, dynrng_power);
  mixer.mix(samples, delay);
  d.process(samples, mixer.out_mode);
}
