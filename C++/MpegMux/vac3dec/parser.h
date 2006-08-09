#ifndef PARSER_H
#define PARSER_H

#include "defs.h"
#include "bitstream.h"

typedef uint8_t exp_t;

struct chinfo_t
{
  int      strtmant;   // channel starting mantissa
  int      endmant;    // channel end mantissa

  int      fastleak;   // fast leak initialisation
  int      slowleak;   // slow leak initialisation
  int      fsnroffst;  // fine snr offset
  int      fgain;      // fast gain

  uint8_t  deltbae;    // delta bit allocation exists
  int8_t   deltba[50]; // per-band delta bit allocation

  int8_t   bap[256];   // derived channel bit allocation
  exp_t    exp[256];   // channel exponents
};


class Parser
{
public:

  // bsi
  uint8_t  frmsizecod;  // frame size code
  uint8_t  fscod;       // sample rate code
  uint8_t  halfrate;    // halfrate flag
  uint8_t  bsid;        // bit stream identification
  uint8_t  acmod;       // audio coding mode (modif)
  uint8_t  dsurmod;     // Dolby Surround mode
  uint8_t  lfeon;       // LFE if on

  uint8_t  dialnorm;    // dialog normalization 
  uint8_t  compre;      // compression gain word exists
  uint8_t  compr;       // compression gain word
  uint8_t  langcode;    // language code exists
  uint8_t  langcod;     // language code
  uint8_t  audprodie;   // audio production information exists
  uint8_t  mixlevel;    // mixing level (SPL)
  uint8_t  roomtyp;     // room type
                        
  uint8_t  dialnorm2;   // dialog normalization 
  uint8_t  compr2e;     // compression gain word exists
  uint8_t  compr2;      // compression gain word
  uint8_t  langcod2e;   // language code exists
  uint8_t  langcod2;    // language code
  uint8_t  audprodi2e;  // audio production information exists
  uint8_t  mixlevel2;   // mixing level (SPL)
  uint8_t  roomtyp2;    // room type

  uint8_t  copyrightb;  // copyright bit
  uint8_t  origbs;      // original bitstream

  struct {
    int hours;
    int mins;
    int secs;
    int frames;
    int fracs;
  } timecode;           // timecode

  int      sample_rate; // sample_rate
  int      bitrate;     // bitrate

  sample_t clev;        // center mix level
  sample_t slev;        // surround mix level
  bool     dynrnge;     // dynamic range gain word exists
  sample_t dynrng;      // dynamic range gain word
  bool     dynrng2e;    // dynamic range gain word exists
  sample_t dynrng2;     // dynamic range gain word


  // block private (reusable information only)
  int      chincpl;     // channels in coupling
  int      ncplbnd;     // number of coupling bands
  int      phsflginu;   // phase flags in use
  uint8_t  rematflg;    // stereo rematrixing
  int      cplbnd[18];  // coupling band end bin numbers
  sample_t cplco[5][18];// coupling cordinates
  uint8_t  dithflag[5]; // dither flag

  chinfo_t cplinfo;     // coupling channel info
  chinfo_t chinfo[5];   // fbw channels info
  chinfo_t lfeinfo;     // lfe channel info

  uint8_t  csnroffst;   // coarse SNR offset
  uint16_t bai;         // bit allocation information

  // block public
  uint8_t  blksw[5];    // block switch
  uint8_t  gainrng[5];  // channel gain range code


  Bitstream bitstream;

  Parser() { reset(); }

  void reset    ();
  void get_bsi(BSI &bsi);

  int  syncinfo (uint8_t *buf);
  int  frame(uint8_t *buf);
  int  parse_block(sample_buffer_t &samples);

  int  parse_exponents (int expstr, int ngrps, exp_t exponent, exp_t *dest);
  int  parse_deltba(int8_t *deltba);
  bool is_zero_snr_offsets();
  void bit_allocate (chinfo_t &chinfo);
};

#endif
