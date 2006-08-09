/*
  Decoder

  AC3 decoder class. Manage Parser, Mixer and IMDCT objects.
  Pull and Push models.

  Usage.

  is_pes       - decode MPEG2 PES stream instead of pure ac3 stream;
  dynrng       - use dynamic range compression (DRC);
  dynrng_power - power of dynamic range compression (dynrng = dynrng^dynrng_power [here 'dynrng' - coefficient from parser])
  clev_lock    - mixer uses clev from parser (only when auto_matrix = true);
  slev_lock    - mixer uses slev from parser (only when auto_matrix = true);
  lfelev_lock  - mixer uses lfelev = 1.0 (only when auto_matrix = true);

  auto_matrix  - use standart mixing matrix recalculated each frame;
  normalize_matrix - normalize mixing matrix for no overflow at output (only when auto_matrix = true);
  bass_redir   - bass redirction; redirects basses from all channels to LFE; works only when it is LFE at output

  frames       - number of decoded frames; total, reset() does not reset it;
  errors       - number of erorrs; total, reset() does not reset it;

  samples      - when block() function called it is 256 decoded samples buffer;
  delay        - delayed samples; do not modify it!

  set_acmod()  - set audio coding mode; virtual; override it to perform specific actions on mode change;
  reset()      - reset decoder, prepare to decode other stream; virtual; override to perform specific actions on stream change;
        
  process_coefs()   - pre-processing in frequency domain; equalizer by default; virtual; override to perform custom coefs processing;
  process_samples() - post-processing in time domain; mixer by default; virtual; override to perform custom sample processing;

  Push model:

  decode()     - decode stream
  frame()      - called each time frame header is decoded; sets mixing params and calls block() 6 times; virtual;
  block()      - called each time block is decoded; pure;


  Pull model:

  frame()      - call it to load next frame into internal buffer;
  block()      - call it to get next 256 samples; next frame is loaded automatically when needed;
  get()        - called from frame() to load portions of data; pure;
*/


#ifndef DECODER_H
#define DECODER_H

#include <stdio.h>
#include <io.h>
#include "defs.h"
#include "parser.h"
#include "mixer.h"
#include "imdct.h"
#include "demux.h"
#include "eq.h"
#include "delay.h"

class PushDecoder
{
protected:
  // frame buffer
  uint8_t  buf[3840];
  // source buffers
  uint8_t *bufptr;      // current data pointer
  uint8_t *bufpos;      // required data pointer

  // sample buffers
  sample_buffer_t samples;
  sample_buffer_t delay;

  Parser   parser;
  Demux    demux;
  Mixer    mixer;
  IMDCT    imdct;
  BSI      bsi;
  Eq       eq;
  Delay    d;


public:                       // defaults:

  bool     is_pes;            // false
  bool     dynrng;            // true
  sample_t dynrng_power;      // 1.0

  bool     clev_lock;         // true
  bool     slev_lock;         // true
  bool     lfelev_lock;       // true
           
  bool     auto_matrix;       // true
  bool     normalize_matrix;  // true
  bool     bass_redir;        // true

  int frames;
  int errors;

  PushDecoder(Speakers spk = Speakers(MODE_STEREO));

  const BSI *get_bsi()              { return &bsi; };
  Speakers   get_mode()             { return mixer.out_mode; };
  void       set_mode(Speakers spk) { mixer.out_mode = spk; mixer.reset(); };

  virtual void reset();

  virtual void decode(uint8_t *buffer, uint32_t length);
  virtual void sync() {};
  virtual void frame();
  virtual void block() {};

  virtual void process_coefs();    // default: equalizer
  virtual void process_samples();  // default: mixer
};


class PullDecoder
{
protected:
	// frame buffer
	uint8_t  buf[3840];
	
	// sample buffers
	sample_buffer_t samples;
	sample_buffer_t delay;
	sample_line_t   equalizer;
	
	Demux    demux;
	Parser   parser;
	Mixer    mixer;
	IMDCT    imdct;
	BSI      bsi;
	Eq       eq;
	Delay    d;
	int      blocks;
	
public:
	bool     is_pes;            // false by default
	bool     dynrng;            // true
	sample_t dynrng_power;      // 1.0
	
	bool     clev_lock;         // true
	bool     slev_lock;         // true
	bool     lfelev_lock;       // true
	
	bool     auto_matrix;       // true
	bool     normalize_matrix;  // true
	
	int frames;
	int errors;
	
	PullDecoder(Speakers spk);
	
	const BSI     *get_bsi()              { return &bsi; }
	const uint8_t *get_frame()            { return buf; }
	Speakers       get_mode()             { return mixer.out_mode; };
	void           set_mode(Speakers spk) { mixer.out_mode = spk; reset(); };
	sample_t*	   getSamples(int ch){ return samples[ch]; }
	int			   getSampleRate(){ return parser.sample_rate; };

	virtual void reset();
	
	virtual long get(uint8_t *buf, int len) = 0;
	virtual bool frame(int max_sync_scan = 0x2000 /* 0 = infinite */);
	virtual bool block();
	
	virtual void process_coefs();    // default: equalizer
	virtual void process_samples();  // default: mixer
};


#endif
