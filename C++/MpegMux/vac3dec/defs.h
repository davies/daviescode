#ifndef VAC3DEC_DEFS_H
#define VAC3DEC_DEFS_H

#include <memory.h>

// base word types
typedef signed char     int8_t;
typedef signed short    int16_t;
typedef signed int      int32_t;
typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned int    uint32_t;

// level multipliers
#define LEVEL_PLUS6DB 2.0
#define LEVEL_PLUS3DB 1.4142135623730951
#define LEVEL_3DB     0.7071067811865476
#define LEVEL_45DB    0.5946035575013605
#define LEVEL_6DB     0.5  

// basic constants
#define NCHANNELS    6
#define NSAMPLES     256
#define MAX_DELAY    48000

// audio sample
typedef double   sample_t;
// block of audio samples 6 channels 256 samples each
typedef sample_t sample_buffer_t[NCHANNELS][NSAMPLES];
// one 256 samples line for one channel
typedef sample_t sample_line_t[NSAMPLES];
// mixing matrix 6-to-6 channels
typedef sample_t mixer_matrix_t[NCHANNELS][NCHANNELS];

// audio coding modes
// equivalent to A52/A standart modes except for DPL, DPLII and SPDIF modes
#define MODE_DUAL    0
#define MODE_1_0     1
#define MODE_2_0     2
#define MODE_3_0     3
#define MODE_2_1     4
#define MODE_3_1     5
#define MODE_2_2     6
#define MODE_3_2     7
#define MODE_LFE     8  // can be combined with any of previous modes
#define MODE_ERROR   16

#define MODE_MONO    1
#define MODE_STEREO  2
#define MODE_QUADRO  6
#define MODE_5_1     (MODE_3_2 | MODE_LFE)

// Dolby encoding mode
#define NO_DOLBY          0  // Stream is not Dolby-encoded
#define DOLBY_SURROUND    1  // Dolby Surround
#define DOLBY_PROLOGIC    2  // Dolby Pro Logic
#define DOLBY_PROLOGICII  3  // Dolby Pro Logic II
#define DOLBY_LFE    	  4  // LFE presence flag

// channel numbers
// used as index in arrays
#define CH_L         0  // Left channel
#define CH_C         1  // Center channel
#define CH_R         2  // Right channel
#define CH_SL        3  // Surround left channel
#define CH_SR        4  // Surround right channel
#define CH_LFE       5  // LFE channel
#define CH_NONE      6  // indicates that channel is not used in channel order

#define CH_M         1  // Mono channel = center channel
#define CH_CH1       0  // Channel 1 in Dual mono mode
#define CH_CH2       2  // Channel 2 in Dual mono mode
#define CH_S         3  // Surround channel for x/1 modes

// channel masks
// used as channel presence flag
#define CH_MASK_L    1
#define CH_MASK_C    2
#define CH_MASK_R    4
#define CH_MASK_SL   8
#define CH_MASK_SR   16
#define CH_MASK_LFE  32
#define CH_MASK_LAST 32

#define CH_MASK_M    2
#define CH_MASK_C1   0
#define CH_MASK_C2   4
#define CH_MASK_S    8
#define CH_MASK(ch)  (1 << (ch))

// ac3 channel masks
static const int _mode_ch[16] =
{
  CH_MASK_C1 | CH_MASK_C2,
  CH_MASK_M,
  CH_MASK_L  | CH_MASK_R,
  CH_MASK_L  | CH_MASK_C   | CH_MASK_R,
  CH_MASK_L  | CH_MASK_R   | CH_MASK_S,
  CH_MASK_L  | CH_MASK_C   | CH_MASK_R   | CH_MASK_S,
  CH_MASK_L  | CH_MASK_R   | CH_MASK_SL  | CH_MASK_SR,
  CH_MASK_L  | CH_MASK_C   | CH_MASK_R   | CH_MASK_SL | CH_MASK_SR,

  CH_MASK_C1 | CH_MASK_C2  | CH_MASK_LFE,
  CH_MASK_M  | CH_MASK_LFE,
  CH_MASK_L  | CH_MASK_R   | CH_MASK_LFE,
  CH_MASK_L  | CH_MASK_C   | CH_MASK_R   | CH_MASK_LFE,
  CH_MASK_L  | CH_MASK_R   | CH_MASK_S   | CH_MASK_LFE,
  CH_MASK_L  | CH_MASK_C   | CH_MASK_R   | CH_MASK_S   | CH_MASK_LFE,
  CH_MASK_L  | CH_MASK_R   | CH_MASK_SL  | CH_MASK_SR  | CH_MASK_LFE,
  CH_MASK_L  | CH_MASK_C   | CH_MASK_R   | CH_MASK_SL  | CH_MASK_SR | CH_MASK_LFE,
};

// ac3 channel order
const _ch_order[16][6] = 
{
  { CH_CH1, CH_CH2,   CH_NONE,  CH_NONE,  CH_NONE,  CH_NONE },
  { CH_M,   CH_NONE,  CH_NONE,  CH_NONE,  CH_NONE,  CH_NONE },
  { CH_L,   CH_R,     CH_NONE,  CH_NONE,  CH_NONE,  CH_NONE },
  { CH_L,   CH_C,     CH_R,     CH_NONE,  CH_NONE,  CH_NONE },
  { CH_L,   CH_R,     CH_S,     CH_NONE,  CH_NONE,  CH_NONE },
  { CH_L,   CH_C,     CH_R,     CH_S,     CH_NONE,  CH_NONE },
  { CH_L,   CH_R,     CH_SL,    CH_SR,    CH_NONE,  CH_NONE },
  { CH_L,   CH_C,     CH_R,     CH_SL,    CH_SR,    CH_NONE },
  { CH_CH1, CH_CH2,   CH_LFE,   CH_NONE,  CH_NONE,  CH_NONE },
  { CH_M,   CH_LFE,   CH_NONE,  CH_NONE,  CH_NONE,  CH_NONE },
  { CH_L,   CH_R,     CH_LFE,   CH_NONE,  CH_NONE,  CH_NONE },
  { CH_L,   CH_C,     CH_R,     CH_LFE,   CH_NONE,  CH_NONE },
  { CH_L,   CH_R,     CH_S,     CH_LFE,   CH_NONE,  CH_NONE },
  { CH_L,   CH_C,     CH_R,     CH_S,     CH_LFE,   CH_NONE },
  { CH_L,   CH_R,     CH_SL,    CH_SR,    CH_LFE,   CH_NONE },
  { CH_L,   CH_C,     CH_R,     CH_SL,    CH_SR,    CH_LFE  }
};


static const int _mode_nfchans[16] =  { 2, 1, 2, 3, 3, 4, 4, 5, 2, 1, 2, 3, 3, 4, 4, 5 };
static const int _mode_nchans[16]  =  { 2, 1, 2, 3, 3, 4, 4, 5, 3, 2, 3, 4, 4, 5, 5, 6 };
static const int _mode_nfront[16]  =  { 2, 1, 2, 3, 2, 3, 2, 3, 2, 1, 2, 3, 2, 3, 2, 3 };
static const int _mode_nrear[16]   =  { 0, 0, 0, 0, 1, 1, 2, 2, 0, 0, 0, 0, 1, 1, 2, 2 };

class Speakers
{
public:
  // audio coding mode, used to specify audio format
  // MODE_XXXX constants and may be flagged with MODE_LFE
  int mode;

  // Dolby-encoded format type
  // one of DOLBY_XXXX constants
  int dolby;

  // channels definition, used to define channel configuration
  // composed of number of CH_MASK_X masks
  int channels;

  // channel order definition
  // array of channel names CH_XXX
  int order[NCHANNELS];

  Speakers(int _mode = MODE_STEREO, int _dolby = NO_DOLBY)
  {
    set_mode(_mode);
    dolby = _dolby;
  }


  inline bool surround() const { return (_mode_nrear[mode&15]) != 0; }
  inline bool lfe()      const { return (mode & MODE_LFE)      != 0; }
  inline bool error()    const { return (mode & ~15)           != 0; }
  inline bool stereo()   const { return (mode &~MODE_LFE) == MODE_STEREO; }

  inline int  acmod()    const { return mode &~MODE_LFE;         }
  inline int  nfchans()  const { return _mode_nfchans[mode&15];  }
  inline int  nchans()   const { return _mode_nchans [mode&15];  }
  inline int  nfront()   const { return _mode_nfront [mode&15];  }
  inline int  nrear()    const { return _mode_nrear  [mode&15];  }

  inline void set_mode(int _mode) { mode = _mode; channels = _mode_ch[mode&15]; memcpy(order, _ch_order[mode&15], sizeof(order)); }
};

/*
// mode macros
#define IS_MODE_SURROUND(mode) (_mode_nrear[(mode)&31])
#define IS_MODE_LFE(mode)      ((mode) & MODE_LFE)
#define IS_MODE_ERROR(mode)    ((mode) >= MODE_ERROR)
#define IS_MODE_STEREO(mode)   ((mode) == MODE_STEREO || (mode) == (MODE_STEREO | MODE_LFE) || (mode) == MODE_DOLBY)
#define MODE_ACMOD(mode)       ((mode) & ~MODE_LFE)
#define MODE_CH(mode)          (_mode_ch     [(mode)&15])
#define MODE_NFCHANS(mode)     (_mode_nfchans[(mode)&15])
#define MODE_NCHANS(mode)      (_mode_nchans [(mode)&31])
#define MODE_NFRONT(mode)      (_mode_nfront [(mode)&31])
#define MODE_NREAR(mode)       (_mode_nrear  [(mode)&31])
//#define NFCHAN2CH(nfchan,mode) ((nfchan)? (nfchan)+(~(mode)&1): 0)
*/

class BSI
{
public:
  /////////////////////////////////////////////////////////
  // frame-wide info
  /////////////////////////////////////////////////////////

  Speakers  spk;
  int       bsid;
  int       frame_size;
  int       bitrate;
  int       sample_rate;

  sample_t  clev;        // center mix level
  sample_t  slev;        // surround mix level

  bool      compre;      // compression gain word exists
  sample_t  compr;       // compression gain word
  bool      compr2e;     // compression gain word 2 exists
  sample_t  compr2;      // compression gain word 2

  int       dialnorm;    // dialog normalization 
  int       dialnorm2;   // dialog normalization 2

  bool      langcode;    // language code exists
  int       langcod;     // language code
  bool      langcod2e;   // language code 2 exists
  int       langcod2;    // language code 2

  bool      audprodie;   // audio production information exists
  int       mixlevel;    // mixing level (SPL)
  int       roomtyp;     // room type
                        
  bool      audprodi2e;  // audio production information 2 exists
  int       mixlevel2;   // mixing level (SPL) 2
  int       roomtyp2;    // room type 2

  bool      copyrightb;  // copyright bit
  bool      origbs;      // original bitstream

  struct {
    int hours;
    int mins;
    int secs;
    int frames;
    int fracs;
  } timecode;           // timecode

  /////////////////////////////////////////////////////////
  // block-wide info
  /////////////////////////////////////////////////////////

  bool      dynrnge;     // dynamic range gain word exists
  sample_t  dynrng;      // dynamic range gain word
  bool      dynrng2e;    // dynamic range gain word 2 exists
  sample_t  dynrng2;     // dynamic range gain word 2

  int       chincpl;     // channels in coupling
  int       cplbegf;     // coupling start frequency
  int       cplendf;     // coupling end frequency
  int       chendf[5];   // channel end frequency
};

#endif
