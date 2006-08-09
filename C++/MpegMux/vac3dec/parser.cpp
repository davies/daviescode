#include <string.h>
#include "parser.h"
#include "dither.h"



const int _acmod_nfchans[8] =  { 2, 1, 2, 3, 3, 4, 4, 5 };
#define ACMOD_NFCHANS(acmod) _acmod_nfchans[(acmod)&7]

#define MANTISSA_BITS 15
#define min(a, b)  (((a) < (b)) ? (a) : (b))


#define EXP_REUSE 0
#define EXP_D15   1
#define EXP_D25   2
#define EXP_D45   3

#define DELTA_BIT_REUSE    0
#define DELTA_BIT_NEW      1
#define DELTA_BIT_NONE     2
#define DELTA_BIT_RESERVED 3



///////////////////////////////////////////////////////////////////////////////
// TABLES
///////////////////////////////////////////////////////////////////////////////

const uint8_t halfrate_tbl[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3};
const int rematrix_band[4] = {25, 37, 61, 253};
const sample_t clev_tbl[4] = {LEVEL_3DB, LEVEL_45DB, LEVEL_6DB, LEVEL_45DB};
const sample_t slev_tbl[4] = {LEVEL_3DB, LEVEL_6DB, 0, LEVEL_6DB};


///////////////////////////////////////////////////////////////////////////////
// scale factor
//
// definition:
//   scale_factor[i] = 1/2^(i+15)
//
// usage: 
//   coef = mantissa * scale_factor[exponent]
//   mantissa - integer value interpreted as fixed point value in range [-1;1]
//   exponent - negative binary exponent
//

const sample_t scale_factor[25] = 
{
  0.000030517578125,
  0.0000152587890625,
  0.00000762939453125,
  0.000003814697265625,
  0.0000019073486328125,
  0.00000095367431640625,
  0.000000476837158203125,
  0.0000002384185791015625,
  0.00000011920928955078125,
  0.000000059604644775390625,
  0.0000000298023223876953125,
  0.00000001490116119384765625,
  0.000000007450580596923828125,
  0.0000000037252902984619140625,
  0.00000000186264514923095703125,
  0.000000000931322574615478515625,
  0.0000000004656612873077392578125,
  0.00000000023283064365386962890625,
  0.000000000116415321826934814453125,
  0.0000000000582076609134674072265625,
  0.00000000002910383045673370361328125,
  0.000000000014551915228366851806640625,
  0.0000000000072759576141834259033203125,
  0.00000000000363797880709171295166015625,
  0.000000000001818989403545856475830078125
};



///////////////////////////////////////////////////////////////////////////////
// symmetric quantization tables
//

const sample_t q3_tbl[3] = 
{
  sample_t(- 2 << MANTISSA_BITS) / 3,
  0,
  sample_t(+ 2 << MANTISSA_BITS) / 3
};

const sample_t q5_tbl[5] = 
{
  sample_t(- 4 << MANTISSA_BITS) / 5,
  sample_t(- 2 << MANTISSA_BITS) / 5,
  0,
  sample_t(+ 2 << MANTISSA_BITS) / 5,
  sample_t(+ 4 << MANTISSA_BITS) / 5
};

const sample_t q7_tbl[7] = 
{
  sample_t(- 6 << MANTISSA_BITS) / 7,
  sample_t(- 4 << MANTISSA_BITS) / 7,
  sample_t(- 2 << MANTISSA_BITS) / 7,
  0,
  sample_t(+ 2 << MANTISSA_BITS) / 7,
  sample_t(+ 4 << MANTISSA_BITS) / 7,
  sample_t(+ 6 << MANTISSA_BITS) / 7
};

const sample_t q11_tbl[11] = 
{
  sample_t(-10 << MANTISSA_BITS) / 11,
  sample_t(- 8 << MANTISSA_BITS) / 11,
  sample_t(- 6 << MANTISSA_BITS) / 11,
  sample_t(- 4 << MANTISSA_BITS) / 11,
  sample_t(- 2 << MANTISSA_BITS) / 11,
  0,        
  sample_t(+ 2 << MANTISSA_BITS) / 11,
  sample_t(+ 4 << MANTISSA_BITS) / 11,
  sample_t(+ 6 << MANTISSA_BITS) / 11,
  sample_t(+ 8 << MANTISSA_BITS) / 11,
  sample_t(+10 << MANTISSA_BITS) / 11,
};

const sample_t q15_tbl[15] = 
{
  sample_t(-14 << MANTISSA_BITS) / 15,
  sample_t(-12 << MANTISSA_BITS) / 15,
  sample_t(-10 << MANTISSA_BITS) / 15,
  sample_t(- 8 << MANTISSA_BITS) / 15,
  sample_t(- 6 << MANTISSA_BITS) / 15,
  sample_t(- 4 << MANTISSA_BITS) / 15,
  sample_t(- 2 << MANTISSA_BITS) / 15,
  0,
  sample_t(+ 2 << MANTISSA_BITS) / 15,
  sample_t(+ 4 << MANTISSA_BITS) / 15,
  sample_t(+ 6 << MANTISSA_BITS) / 15,
  sample_t(+ 8 << MANTISSA_BITS) / 15,
  sample_t(+10 << MANTISSA_BITS) / 15,
  sample_t(+12 << MANTISSA_BITS) / 15,
  sample_t(+14 << MANTISSA_BITS) / 15
};


///////////////////////////////////////////////////////////////////////////////
// exponent decoding tables
// 

const int8_t exp_1[128] = {
    -2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
     2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    25,25,25
};

const int8_t exp_2[128] = {
    -2,-2,-2,-2,-2,-1,-1,-1,-1,-1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
    -2,-2,-2,-2,-2,-1,-1,-1,-1,-1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
    -2,-2,-2,-2,-2,-1,-1,-1,-1,-1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
    -2,-2,-2,-2,-2,-1,-1,-1,-1,-1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
    -2,-2,-2,-2,-2,-1,-1,-1,-1,-1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
    25,25,25
};

const int8_t exp_3[128] = {
    -2,-1, 0, 1, 2,-2,-1, 0, 1, 2,-2,-1, 0, 1, 2,-2,-1, 0, 1, 2,-2,-1, 0, 1, 2,
    -2,-1, 0, 1, 2,-2,-1, 0, 1, 2,-2,-1, 0, 1, 2,-2,-1, 0, 1, 2,-2,-1, 0, 1, 2,
    -2,-1, 0, 1, 2,-2,-1, 0, 1, 2,-2,-1, 0, 1, 2,-2,-1, 0, 1, 2,-2,-1, 0, 1, 2,
    -2,-1, 0, 1, 2,-2,-1, 0, 1, 2,-2,-1, 0, 1, 2,-2,-1, 0, 1, 2,-2,-1, 0, 1, 2,
    -2,-1, 0, 1, 2,-2,-1, 0, 1, 2,-2,-1, 0, 1, 2,-2,-1, 0, 1, 2,-2,-1, 0, 1, 2,
    25,25,25
};





































///////////////////////////////////////////////////////////////////////////////
//
// Parser class
//
///////////////////////////////////////////////////////////////////////////////

void 
Parser::reset()
{
  dynrng = 1.0;
  dynrng2= 1.0;
  clev = 1.0;
  slev = 1.0;
  memset(&timecode, 0, sizeof(timecode));
}


void 
Parser::get_bsi(BSI &bsi)
{
  const int rate[] = { 32,  40,  48,  56,  64,  80,  96, 112,
                       128, 160, 192, 224, 256, 320, 384, 448,
                       512, 576, 640 };

  switch (fscod) 
  {
    case 0:   bsi.frame_size = 4 * rate[frmsizecod >> 1]; break;
    case 1:   bsi.frame_size = 2 * (320 * rate[frmsizecod >> 1] / 147 + (frmsizecod & 1)); break;
    case 2:   bsi.frame_size = 6 * rate[frmsizecod >> 1]; break;
  }


  bsi.spk         = Speakers(lfeon? acmod | MODE_LFE: acmod, dsurmod == 2? DOLBY_SURROUND: NO_DOLBY);
  bsi.bsid        = bsid;
  bsi.bitrate     = bitrate;
  bsi.sample_rate = sample_rate;

  bsi.clev        = clev;
  bsi.slev        = slev;

  bsi.compre      = compre != 0;         
  bsi.compr       = compr;         /// !!!!
  bsi.compr2e     = compr2e != 0;
  bsi.compr2      = compr2;

  bsi.dialnorm    = dialnorm;
  bsi.dialnorm2   = dialnorm2;

  bsi.langcode    = langcode != 0;
  bsi.langcod     = langcod;
  bsi.langcod2e   = langcod2e != 0;
  bsi.langcod2    = langcod2;

  bsi.audprodie   = audprodie != 0;
  bsi.mixlevel    = mixlevel;
  bsi.roomtyp     = roomtyp;
                         
  bsi.audprodi2e  = audprodi2e != 0;
  bsi.mixlevel2   = mixlevel2;
  bsi.roomtyp2    = roomtyp2;
                         
  bsi.copyrightb  = copyrightb != 0;
  bsi.origbs      = origbs != 0;

  memcpy(&bsi.timecode, &timecode, sizeof(timecode));

  bsi.dynrnge     = dynrnge;  
  bsi.dynrng      = dynrng;   
  bsi.dynrng2e    = dynrng2e; 
  bsi.dynrng2     = dynrng2;  

  double bin2freq = sample_rate / 512.0;
  bsi.chincpl     = chincpl;
  bsi.cplbegf     = int(cplinfo.strtmant  * bin2freq);
  bsi.cplendf     = int(cplinfo.endmant   * bin2freq);
  bsi.chendf[0]   = chincpl & 1?  bsi.cplendf: int(chinfo[0].endmant * bin2freq);
  bsi.chendf[1]   = chincpl & 2?  bsi.cplendf: int(chinfo[1].endmant * bin2freq);
  bsi.chendf[2]   = chincpl & 4?  bsi.cplendf: int(chinfo[2].endmant * bin2freq);
  bsi.chendf[3]   = chincpl & 8?  bsi.cplendf: int(chinfo[3].endmant * bin2freq);
  bsi.chendf[4]   = chincpl & 16? bsi.cplendf: int(chinfo[4].endmant * bin2freq);
}



int       
Parser::syncinfo (uint8_t * buf)
{
  const int rate[] = { 32,  40,  48,  56,  64,  80,  96, 112,
                       128, 160, 192, 224, 256, 320, 384, 448,
                       512, 576, 640 };
  int frmsizecod, frmsize;
  int half;

  
  if ((buf[0] != 0x0b) || (buf[1] != 0x77))      // 'syncword' - syncronisation word
    return 0;
  
  if (buf[5] >= 0x60)                            // 'bsid' - bit stream identfication
    return 0;
  
  half = halfrate_tbl[buf[5] >> 3];

  frmsizecod = buf[4] & 0x3f;
  if (frmsizecod > 0x25)
    return 0;

  frmsize = rate [frmsizecod >> 1];
  bitrate = (frmsize * 1000) >> half;

  switch (buf[4] & 0xc0) 
  {
    case 0:    sample_rate = 48000 >> half;  
      return 4 * frmsize;
    case 0x40: sample_rate = 44100 >> half;  
      return 2 * (320 * frmsize / 147 + (frmsizecod & 1));
    case 0x80: sample_rate = 32000 >> half;  
      return 6 * frmsize;
    default:
      return 0;
  }
}










int 
Parser::frame(uint8_t * buf)
{
  /////////////////////////////////////////////////////////////
  // Parse bit stream information (BSI)
  /////////////////////////////////////////////////////////////

  frmsizecod = buf[4] & 0x3f;          // 'frmsizecod' - frame size code
  fscod    = buf[4] >> 6;              // 'fscod' - sample rate code
  bsid     = buf[5] >> 3;              // 'bsid' - bitstream id
  halfrate = halfrate_tbl[bsid];       
  acmod    = buf[6] >> 5;              
  
  bitstream.set_ptr (buf + 6);         // reset bitsteram pointer

  acmod = bitstream.get (3);           // 'acmod' - audio coding mode

  if ((acmod & 1) && (acmod != 1))
    clev = clev_tbl[bitstream.get(2)]; // 'clev' - center mix level

  if (acmod & 4)
    slev = slev_tbl[bitstream.get(2)]; // 'slev' - surround mix level

  if (acmod == MODE_STEREO)
    dsurmod = bitstream.get (2);       // 'dsurmod' - Dolby Surround mode
  else
    dsurmod = 0;

  lfeon     = bitstream.get (1);       // 'lfeon' - flag shows if it is LFE channel in stream
  dialnorm  = bitstream.get (5);       // 'dialnorm' - dialog normalization

  compre    = bitstream.get (1);       // 'compre' - compression gain word
  if (compre)
    compr   = bitstream.get (8);       // 'compr' - compression gain word

  langcode  = bitstream.get (1);       // 'langcode' - language code exists
  if (langcode)
    langcod = bitstream.get (8);       // 'langcod' - language code

  audprodie = bitstream.get (1);       // 'audprodie' - audio production information exists
  if (audprodie)
  {
    mixlevel = bitstream.get(5) + 80;  // 'mixlevel' - mixing level in SPL
    roomtyp  = bitstream.get(2);       // 'roomtyp' - room type
  }

  if (acmod == MODE_DUAL)
  {
    dialnorm  = bitstream.get (5);     // 'dialnorm2' - dialog normalization

    compre    = bitstream.get (1);     // 'compr2e' - compression gain word
    if (compre)
      compr   = bitstream.get (8);     // 'compr2' - compression gain word

    langcode  = bitstream.get (1);     // 'langcod2e' - language code exists
    if (langcod)
      langcod = bitstream.get (8);     // 'langcod2' - language code

    audprodie = bitstream.get (1);     // 'audprodi2e' - audio production information exists
    if (audprodie)
    {
      mixlevel = bitstream.get(5) + 80;// 'mixlevel2' - mixing level in SPL
      roomtyp  = bitstream.get(2);     // 'roomtyp2' - room type
    }
  }

  copyrightb = bitstream.get(1);       // 'copyrightb' - copyright bit
  origbs = bitstream.get (1);          // 'origbs' - original bitstream

  if (bitstream.get (1))               // 'timecod1e' - timecode first half exists
  {
    timecode.hours = bitstream.get(5);
    timecode.mins  = bitstream.get(6);
    timecode.secs  = bitstream.get(3) << 4;
  }
  if (bitstream.get (1))               // 'timecod2e' - timecode second half exists
  {
    timecode.secs  += bitstream.get(3);
    timecode.frames = bitstream.get(5);
    timecode.fracs  = bitstream.get(6);
  }
  
  if (bitstream.get (1))               // 'addbsie' - additional bitstream information exists
  {
    int addbsil = bitstream.get (6);   // 'addbsil' - additioanl bitstream information length
    do 
    {
      bitstream.get (8);               // 'addbsi' - additional bitstream information
    } while (addbsil--);
  }

  /////////////////////////////////////////////////////////////
  // Init variables to for first block decoding
  /////////////////////////////////////////////////////////////

  cplinfo.deltbae = DELTA_BIT_NONE;
  chinfo[0].deltbae = chinfo[1].deltbae = 
  chinfo[2].deltbae = chinfo[3].deltbae = chinfo[4].deltbae = DELTA_BIT_NONE;

//  dynrng = 1.0;
//  dynrng2 = 1.0;

  return 0;
}










int 
Parser::parse_block(sample_buffer_t &samples)
{
  // block 
  int ch, bnd;
  int do_bit_alloc;
  int nfchans = ACMOD_NFCHANS(acmod);

  /////////////////////////////////////////////////////////////
  // Parse bitstream
  /////////////////////////////////////////////////////////////

  for (ch = 0; ch < nfchans; ch++)
    blksw[ch] = bitstream.get (1);               // 'blksw[ch]' - block switch

  for (ch = 0; ch < nfchans; ch++)
    dithflag[ch] = bitstream.get (1);            // 'dithflag[ch]' - dither flag

  if (bitstream.get(1))                          // 'dynrnge' - dynamic range gain word exists
  {
    uint32_t dynrng_word = bitstream.get(8);     // 'dynrng' - dynamic range gain word
    dynrng = (((dynrng_word & 0x1f) | 0x20) << 13) * scale_factor[(3 - (dynrng_word >> 5)) & 7];
  }                                               

  if (acmod == MODE_DUAL && bitstream.get(1))    // 'dynrng2e' - dynamic range gain word 2 exists
  {
    uint32_t dynrng2_word = bitstream.get(8);    // 'dynrng2' - dynamic range gain word 2
    dynrng2 = ((((dynrng2_word & 0x1f) | 0x20) << 13) * scale_factor[(3 - (dynrng2_word >> 5)) & 7]);
  }

  /////////////////////////////////////////////////////////////
  // Coupling information
  /////////////////////////////////////////////////////////////
  
  // in:  acmod nfchans
  // out: chincpl ncplbnd phsflginu cplinfo.strtmant cplinfo.endmant

  if (bitstream.get (1))                         // 'cplstre' - coupling strategy exists
  {
    chincpl = 0;
    if (bitstream.get (1))                       // 'cplinu' - coupling in use
    {
      int ncplsubnd;

      for (ch = 0; ch < nfchans; ch++)           // 'chincpl' - channel in coupling flag for each channel
        chincpl |= bitstream.get (1) << ch; 

      switch (acmod)
      {
        case MODE_DUAL: 
        case MODE_MONO:
          // this modes are not allowed for coupling
          return 1;

        case MODE_STEREO:          
          phsflginu = bitstream.get (1);         // 'phsflginu' - phase flag in use
      }

      int cplbegf = bitstream.get (4);           // 'cplbegf' - coupling begin frequency code
      int cplendf = bitstream.get (4);           // 'cplendf' - coupling begin frequency code
      if (cplendf + 3 - cplbegf < 0)             // check if the end band is before begin
        return 1;

      // calc 'ncplbnd' - number of sub-bands in coupling
      ncplbnd = ncplsubnd = cplendf + 3 - cplbegf;
      cplinfo.strtmant = cplbegf * 12 + 37;
      cplinfo.endmant  = cplendf * 12 + 73;

      cplbnd[0] = cplinfo.strtmant + 12;
      ncplbnd = 0;
      for (bnd = 0; bnd < ncplsubnd - 1; bnd++)
        if (bitstream.get (1)) 
          cplbnd[ncplbnd] += 12;
        else
        {
          ncplbnd++;
          cplbnd[ncplbnd] = cplbnd[ncplbnd-1] + 12;
        }
      ncplbnd++;   // coupling band index to number to coupling bands

    }
  }

  /////////////////////////////////////////////////////////////
  // Coupling coordinates
  /////////////////////////////////////////////////////////////
  
  // in:  acmod nfchans chincpl ncplbnd phsflginu
  // out: cplco[][]

  if (chincpl) 
  {             
    int cplcoe = 0;                              // 'cplcoe' - coupling coorinates exists

    for (ch = 0; ch < nfchans; ch++)
      if ((chincpl >> ch) & 1)        
        if (bitstream.get (1))                   // 'cplcoe' - coupling coorinates exists
        {
          int mstrcplco;                         // 'mstrcplco' - master coupling coordinate
          int cplcoexp;                          // 'cplcoexp' - coupling coordinate exponent
          int cplcomant;                         // 'cplcomant' - coupling coordinate mantissa

          cplcoe = 1;          
          mstrcplco = bitstream.get (2) * 3;     // 'mstrcplco' - master coupling coordinate

          for (bnd = 0; bnd < ncplbnd; bnd++) 
          {            
            cplcoexp = bitstream.get (4);        // 'cplcoexp' - coupling coordinate exponent            
            cplcomant = bitstream.get (4);       // 'cplcomant' - coupling coordinate mantissa

            if (cplcoexp == 15)
              cplcomant <<= 14;
            else
              cplcomant = (cplcomant | 0x10) << 13;

            cplco[ch][bnd] = cplcomant * scale_factor[cplcoexp + mstrcplco];
          }
        }

    // if 2/x mode & coupling & phase flag in use then
    // read read phase flags for each band
    if (acmod == MODE_STEREO && phsflginu && cplcoe)
      for (bnd = 0; bnd < ncplbnd; bnd++)
        if (bitstream.get (1))
          cplco[1][bnd] = -cplco[ch][bnd];       // 'phsflg' - phase flag
  }

  /////////////////////////////////////////////////////////////
  // Rematrixing
  /////////////////////////////////////////////////////////////

  // in:  acmod chincpl cplinfo.strtmant
  // out: rematflg

  if (acmod == MODE_STEREO && bitstream.get(1)) 
  {
    int end;

    rematflg = 0;

    end = (chincpl) ? cplinfo.strtmant : 253;  
    bnd = 0;
    do      
      rematflg |= bitstream.get (1) << bnd;       // 'rematflg' - rematrix flag
    while (rematrix_band[bnd++] < end);
  }

  /////////////////////////////////////////////////////////////
  // Exponent decoding
  /////////////////////////////////////////////////////////////

  // out: do_bit_alloc chinfo.endmant cplcoef[].exp coef[][].exp lfecoef[].exp

  do_bit_alloc = 0;

  {
    int cplexpstr = EXP_REUSE; // =?
    int lfeexpstr = EXP_REUSE; // =?
    int chexpstr[5];

    // exponent strategy

    if (chincpl) 
      cplexpstr = bitstream.get (2);             // 'cplexpstr' - coupling exponent strategy

    for (ch = 0; ch < nfchans; ch++)
      chexpstr[ch] = bitstream.get (2);          // 'chexpstr' - channel exponent strategy for each fbw channel

    if (lfeon) 
      lfeexpstr = bitstream.get (1);             // 'lfeexpstr' - LFE exponent strategy

    // chinfo[].strtmant, chinfo[].endmant

    for (ch = 0; ch < nfchans; ch++)
      if (chexpstr[ch] != EXP_REUSE) 
      {
        chinfo[ch].strtmant = 0;
        if ((chincpl >> ch) & 1)
          // if channel is coupled then use coupling start mantissa number
          chinfo[ch].endmant = cplinfo.strtmant;
        else 
        {
          int chbwcod;
          chbwcod = bitstream.get (6);           // 'chbwcod' = channel bandwidth code
          if (chbwcod > 60)
            return 1;
          chinfo[ch].endmant = chbwcod * 3 + 73;
        }
      }
    lfeinfo.strtmant = 0;
    lfeinfo.endmant = 7;


    // coupling channel exponents

    if (cplexpstr != EXP_REUSE) 
    {
      do_bit_alloc = 64;

      // 'ncplgrps' - number of coupling groups
      int grp_size = 3 << (cplexpstr - 1);
      int ncplgrps = (cplinfo.endmant - cplinfo.strtmant) / grp_size;
      exp_t cplabsexp = bitstream.get (4) << 1;  // 'cplabsexp' - coupling absolute exponents
      if (parse_exponents (cplexpstr, ncplgrps, cplabsexp, cplinfo.exp + cplinfo.strtmant))
        return 1;
    }

    // parse exponents for fbw channels

    for (ch = 0; ch < nfchans; ch++)
      if (chexpstr[ch] != EXP_REUSE) 
      {
        do_bit_alloc |= 1 << ch;

        int grp_size = 3 << (chexpstr[ch] - 1);
        int nchgrps = (chinfo[ch].endmant + grp_size - 4) / grp_size;
        chinfo[ch].exp[0] = bitstream.get (4);   // 'exps[0]' - absolute exponent
        if (parse_exponents (chexpstr[ch], nchgrps, chinfo[ch].exp[0], chinfo[ch].exp + 1))
          return 1;
      
        gainrng[ch] = bitstream.get (2);         // 'gainrng'  - channel gain range code
      }

    // parse lfe exponents

    if (lfeexpstr != EXP_REUSE) 
    {
      do_bit_alloc |= 32;
    
      // prase 2 diffenrential exponent groups for lfe
      lfeinfo.exp[0] = bitstream.get (4);        // 'lfeexps[0]' - absolute exponent
      if (parse_exponents (lfeexpstr, 2, lfeinfo.exp[0], lfeinfo.exp + 1))
        return 1;
    }
  }

  /////////////////////////////////////////////////////////////
  // Bit allocation information
  /////////////////////////////////////////////////////////////
  
  if (bitstream.get (1))                         // 'baie' - bit allication information exists
  {
    do_bit_alloc = -1;
    
    bai = bitstream.get (11);                    // bit allocation information
//    sdecay = slowdec[bitstream.get(2)];          // 'sdcycod' - slow decay code
//    fdecay = fastdec[bitstream.get(2)];          // 'fdcycod' - fast decay code
//    sgain  = slowgain[bitstream.get(2)];         // 'sgaincod' - slow gain code
//    dbknee = dbpbtab[bitstream.get(2)];          // 'dbpbcod' - Db per bit code
//    floor  = floortab[bitstream.get(3)];         // 'floorcod' - masking floor code
  }

  const int fastgain[8] = { 0x080, 0x100, 0x180, 0x200, 0x280, 0x300, 0x380, 0x400 };
  if (bitstream.get (1))                         // 'snroffste' - SNR offset exists
  {
    do_bit_alloc = -1;
    
    csnroffst = bitstream.get (6);               // 'csnroffst' - coarse SNR offset
    if (chincpl)
    {
      cplinfo.fsnroffst = bitstream.get(4);      // 'cplfsnroffst' - coupling fine SNR offset
      cplinfo.fgain = fastgain[bitstream.get(3)];// 'cplfgaincod' - coupling fast gain code
    }

    for (ch = 0; ch < nfchans; ch++)
    {      
      chinfo[ch].fsnroffst = bitstream.get(4);        // 'fsnroffst' - channel fine SNR offset
      chinfo[ch].fgain = fastgain[bitstream.get(3)];  // 'fgaincod' - channel fast gain code
    }

    if (lfeon)
    {      
      lfeinfo.fsnroffst = bitstream.get(4);           // 'lfesnroffst' - LFE channel SNR offset
      lfeinfo.fgain = fastgain[bitstream.get(3)];     // 'lfegaincod' - LFE channel gain code
    }
  }

  
  if ((chincpl) && (bitstream.get (1)))               // 'cplleake' - coupling leak initialization exists
  {
    do_bit_alloc |= 64;
    cplinfo.fastleak = (9 - bitstream.get(3)) << 8;   // 'cplfleak' - coupling fast leak initialization
    cplinfo.slowleak = (9 - bitstream.get(3)) << 8;   // 'cplsleak' - coupling slow leak initialization
  } 
  
  if (bitstream.get (1))                         // 'deltbaie' - delta bit allication information exists
  {
    do_bit_alloc = -1;

    if (chincpl)
      cplinfo.deltbae = bitstream.get (2);       // 'cpldeltbae' - couling delta bit allocation information exists

    for (ch = 0; ch < nfchans; ch++)
      chinfo[ch].deltbae = bitstream.get (2);    // 'deltbae' - delta bit allocation exists

    // parse delta bit information for coupling channels
    if (chincpl && (cplinfo.deltbae == DELTA_BIT_NEW) && parse_deltba (cplinfo.deltba))
      return 1;

    // parse delta bit information for fbw channels
    for (ch = 0; ch < nfchans; ch++)
      if ((chinfo[ch].deltbae == DELTA_BIT_NEW) && parse_deltba (chinfo[ch].deltba))
        return 1;
  }

  /////////////////////////////////////////////////////////////
  // Skip information
  /////////////////////////////////////////////////////////////
  
  // 'skiple' - skip length exists
  if (bitstream.get (1)) 
  {
    // 'skipl' - slkip length
    ch = bitstream.get (9);
    // skip
    while (ch--)
      bitstream.get (8);
  }

  /////////////////////////////////////////////////////////////
  // Perform bit allocation
  /////////////////////////////////////////////////////////////

  if (do_bit_alloc) 
  {
    if (is_zero_snr_offsets ())
    {
      memset (cplinfo.bap, 0, sizeof (cplinfo.bap));

      for (ch = 0; ch < nfchans; ch++)
        memset (chinfo[ch].bap, 0, sizeof (chinfo[ch].bap));

      memset (lfeinfo.bap, 0, sizeof (lfeinfo.bap));
    } 
    else 
    {
      if (chincpl && (do_bit_alloc & 64))
        bit_allocate (cplinfo);

      for (ch = 0; ch < nfchans; ch++)
        if (do_bit_alloc & (1 << ch))
        {
          chinfo[ch].strtmant = 0;
          chinfo[ch].fastleak = 0;
          chinfo[ch].slowleak = 0;
          bit_allocate (chinfo[ch]);
        }

      if (lfeon && (do_bit_alloc & 32)) 
      {
        lfeinfo.strtmant = 0;
        lfeinfo.fastleak = 0;
        lfeinfo.slowleak = 0;
        lfeinfo.deltbae = DELTA_BIT_NONE;
        bit_allocate (lfeinfo);
      }
    }
  }

  /////////////////////////////////////////////////////////////
  // Parse mantissas
  /////////////////////////////////////////////////////////////

  {
    int n, endmant;
    int got_cplchan = 0;

    sample_t q3[2];
    sample_t q5[2];
    sample_t q11;
    int      nq3  = -1;
    int      nq5  = -1;
    int      nq11 = -1;

    #define get_coef(coef, exp, bap, dithflag)                   \
    {                                                            \
      switch (bap)                                               \
      {                                                          \
        case -1:                                                 \
          if (nq3 >= 0)                                          \
            coef = q3[nq3--] * scale_factor[exp];                \
          else                                                   \
          {                                                      \
            int code = bitstream.get(5);                         \
            nq3 = 1;                                             \
            q3[0] = q3_tbl[(code%9)%3];                          \
            q3[1] = q3_tbl[(code%9)/3];                          \
            coef = q3_tbl[code/9] * scale_factor[exp];           \
          }                                                      \
          break;                                                 \
                                                                 \
        case -2:                                                 \
          if (nq5 >= 0)                                          \
            coef = q5[nq5--] * scale_factor[exp];                \
          else                                                   \
          {                                                      \
            int code = bitstream.get(7);                         \
            nq5 = 1;                                             \
            q5[0] = q5_tbl[(code%25)%5];                         \
            q5[1] = q5_tbl[(code%25)/5];                         \
            coef = q5_tbl[code/25] * scale_factor[exp];          \
          }                                                      \
          break;                                                 \
                                                                 \
                                                                 \
        case 3:                                                  \
          coef = q7_tbl[bitstream.get(3)] * scale_factor[exp];   \
          break;                                                 \
                                                                 \
        case -3:                                                 \
          if (nq11 >= 0)                                         \
          {                                                      \
            nq11--;                                              \
            coef = q11 * scale_factor[exp];                      \
          }                                                      \
          else                                                   \
          {                                                      \
            int code = bitstream.get(7);                         \
            nq11 = 0;                                            \
            q11 = q11_tbl[code%11];                              \
            coef = q11_tbl[code/11] * scale_factor[exp];         \
          }                                                      \
          break;                                                 \
                                                                 \
        case 4:                                                  \
          coef = q15_tbl[bitstream.get(4)] * scale_factor[exp];  \
          break;                                                 \
                                                                 \
        case 0:                                                  \
          if (dithflag)                                          \
            coef = dither_gen() * LEVEL_3DB * scale_factor[exp]; \
          else                                                   \
            coef = 0;                                            \
          break;                                                 \
                                                                 \
        default:                                                 \
          coef = (bitstream.get_signed(bap) << (16 - bap)) * scale_factor[exp];  \
          break;                                                 \
      }                                                          \
    }

    for (ch = 0; ch < nfchans; ch++)
    {
      sample_t *coef = samples[ch] + chinfo[ch].strtmant;
      for (n = chinfo[ch].strtmant; n < chinfo[ch].endmant; n++)    
        get_coef(samples[ch][n], chinfo[ch].exp[n], chinfo[ch].bap[n], dithflag[ch]);

      if ((chincpl >> ch) & 1)
      {
        if (!got_cplchan)
        {
          got_cplchan = 1;

          int cplch;
          sample_t coef;

          n = cplinfo.strtmant;
          for (bnd = 0; bnd < ncplbnd; bnd++)
            for (; n < cplbnd[bnd]; n++)
              if (cplinfo.bap[n])
              {
                get_coef(coef, cplinfo.exp[n], cplinfo.bap[n], 0);
                for (cplch = 0; cplch < nfchans; cplch++)
                  if ((chincpl >> cplch ) &  1)
                    samples[cplch][n] = coef * cplco[cplch][bnd];
              }
              else
              {
                for (cplch = 0; cplch < nfchans; cplch++)
                  if ((chincpl >> cplch ) &  1)
                    if (dithflag[cplch])
                      samples[cplch][n] = dither_gen() * cplco[cplch][bnd] * LEVEL_3DB * scale_factor[cplinfo.exp[n]];
                    else
                      samples[cplch][n] = 0.0;
              }
        }
        endmant = cplinfo.endmant;
      }
      else
        endmant = chinfo[ch].endmant;

      do
        samples[ch][endmant] = 0;
      while (++endmant < 256);

    }

    if (lfeon)
    {
      memset(samples[nfchans], 0, sizeof(samples[nfchans]));
      for (n = lfeinfo.strtmant; n < lfeinfo.endmant; n++)
        get_coef(samples[nfchans][n], lfeinfo.exp[n], lfeinfo.bap[n], 0);
    }
  }

  /////////////////////////////////////////////////////////////
  // Rematrixing
  /////////////////////////////////////////////////////////////

  if (acmod == MODE_STEREO) 
  {
    int bin = 13;
    int bnd = 0;
    int band_end = 0;
    int last_bin = min(chinfo[0].endmant, chinfo[1].endmant);
    int remat = rematflg;
    do
    {
      if (!(remat & 1))
      {
        remat >>= 1;
        bin = rematrix_band[bnd++];
        continue;
      }
      remat >>= 1;
      band_end = rematrix_band[bnd++];

      if (band_end > last_bin)
        band_end = last_bin;

      do 
      {
        sample_t tmp0 = samples[0][bin];
        sample_t tmp1 = samples[1][bin];
        samples[0][bin] = tmp0 + tmp1;
        samples[1][bin] = tmp0 - tmp1;
      } while (++bin < band_end);
    } while (bin < last_bin);
  }

  return 0;
}










int 
Parser::parse_exponents (int expstr, int ngrps, exp_t exponent, exp_t *dest)
{
  int exps;

  while (ngrps--) 
  {
    exps = bitstream.get (7);

    exponent += exp_1[exps];
    if (exponent > 24)
      return 1;

    switch (expstr) 
    {
      case EXP_D45:
        *(dest++) = exponent;
        *(dest++) = exponent;
      case EXP_D25:
        *(dest++) = exponent;
      case EXP_D15:
        *(dest++) = exponent;
    }            

    exponent += exp_2[exps];
    if (exponent > 24)
      return 1;

    switch (expstr) 
    {
      case EXP_D45:
        *(dest++) = exponent;
        *(dest++) = exponent;
      case EXP_D25:
        *(dest++) = exponent;
      case EXP_D15:
        *(dest++) = exponent;
    }

    exponent += exp_3[exps];
    if (exponent > 24)
      return 1;

    switch (expstr) 
    {
      case EXP_D45:
        *(dest++) = exponent;
        *(dest++) = exponent;
      case EXP_D25:
        *(dest++) = exponent;
      case EXP_D15:
        *(dest++) = exponent;
    }
  }     
  return 0;
}










int 
Parser::parse_deltba (int8_t *deltba)
{
  int deltnseg, deltlen, delta, j;

  memset (deltba, 0, 50);

  deltnseg = bitstream.get (3);
  j = 0;
  do 
  {
    j += bitstream.get (5);
    deltlen = bitstream.get (4);
    delta = bitstream.get (3);
    delta -= (delta >= 4) ? 3 : 4;
    if (!deltlen)
      continue;
    if (j + deltlen >= 50)
      return 1;
    while (deltlen--)
      deltba[j++] = delta;
  } while (deltnseg--);

  return 0;
}










inline bool
Parser::is_zero_snr_offsets ()
{
  if ((csnroffst) ||
      (chincpl && cplinfo.fsnroffst >> 3) ||
      (lfeon   && lfeinfo.fsnroffst >> 3))
    return false;

  for (int i = 0; i < ACMOD_NFCHANS(acmod); i++)
    if (chinfo[i].fsnroffst >> 3)
      return false;
  return true;
}









///////////////////bit allocate///////////////////////////////

static const int hthtab[3][50] = {
    {0x730, 0x730, 0x7c0, 0x800, 0x820, 0x840, 0x850, 0x850, 0x860, 0x860,
     0x860, 0x860, 0x860, 0x870, 0x870, 0x870, 0x880, 0x880, 0x890, 0x890,
     0x8a0, 0x8a0, 0x8b0, 0x8b0, 0x8c0, 0x8c0, 0x8d0, 0x8e0, 0x8f0, 0x900,
     0x910, 0x910, 0x910, 0x910, 0x900, 0x8f0, 0x8c0, 0x870, 0x820, 0x7e0,
     0x7a0, 0x770, 0x760, 0x7a0, 0x7c0, 0x7c0, 0x6e0, 0x400, 0x3c0, 0x3c0},
    {0x710, 0x710, 0x7a0, 0x7f0, 0x820, 0x830, 0x840, 0x850, 0x850, 0x860,
     0x860, 0x860, 0x860, 0x860, 0x870, 0x870, 0x870, 0x880, 0x880, 0x880,
     0x890, 0x890, 0x8a0, 0x8a0, 0x8b0, 0x8b0, 0x8c0, 0x8c0, 0x8e0, 0x8f0,
     0x900, 0x910, 0x910, 0x910, 0x910, 0x900, 0x8e0, 0x8b0, 0x870, 0x820,
     0x7e0, 0x7b0, 0x760, 0x770, 0x7a0, 0x7c0, 0x780, 0x5d0, 0x3c0, 0x3c0},
    {0x680, 0x680, 0x750, 0x7b0, 0x7e0, 0x810, 0x820, 0x830, 0x840, 0x850,
     0x850, 0x850, 0x860, 0x860, 0x860, 0x860, 0x860, 0x860, 0x860, 0x860,
     0x870, 0x870, 0x870, 0x870, 0x880, 0x880, 0x880, 0x890, 0x8a0, 0x8b0,
     0x8c0, 0x8d0, 0x8e0, 0x8f0, 0x900, 0x910, 0x910, 0x910, 0x900, 0x8f0,
     0x8d0, 0x8b0, 0x840, 0x7f0, 0x790, 0x760, 0x7a0, 0x7c0, 0x7b0, 0x720}
};

static const int8_t baptab[305] = {
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, /* 93 padding elems */

    16, 16, 16, 16, 16, 16, 16, 16, 16, 14, 14, 14, 14, 14, 14, 14,
    14, 12, 12, 12, 12, 11, 11, 11, 11, 10, 10, 10, 10,  9,  9,  9,
     9,  8,  8,  8,  8,  7,  7,  7,  7,  6,  6,  6,  6,  5,  5,  5,
     5,  4,  4, -3, -3,  3,  3,  3, -2, -2, -1, -1, -1, -1, -1,  0,

     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  0,  0,  0                                      /* 148 padding elems */
};

static const int bndtab[30] = {21, 22,  23,  24,  25,  26,  27,  28,  31,  34,
                         37, 40,  43,  46,  49,  55,  61,  67,  73,  79,
                         85, 97, 109, 121, 133, 157, 181, 205, 229, 253};

static const int8_t latab[256] = {
    -64, -63, -62, -61, -60, -59, -58, -57, -56, -55, -54, -53,
    -52, -52, -51, -50, -49, -48, -47, -47, -46, -45, -44, -44,
    -43, -42, -41, -41, -40, -39, -38, -38, -37, -36, -36, -35,
    -35, -34, -33, -33, -32, -32, -31, -30, -30, -29, -29, -28,
    -28, -27, -27, -26, -26, -25, -25, -24, -24, -23, -23, -22,
    -22, -21, -21, -21, -20, -20, -19, -19, -19, -18, -18, -18,
    -17, -17, -17, -16, -16, -16, -15, -15, -15, -14, -14, -14,
    -13, -13, -13, -13, -12, -12, -12, -12, -11, -11, -11, -11,
    -10, -10, -10, -10, -10,  -9,  -9,  -9,  -9,  -9,  -8,  -8,
     -8,  -8,  -8,  -8,  -7,  -7,  -7,  -7,  -7,  -7,  -6,  -6,
     -6,  -6,  -6,  -6,  -6,  -6,  -5,  -5,  -5,  -5,  -5,  -5,
     -5,  -5,  -4,  -4,  -4,  -4,  -4,  -4,  -4,  -4,  -4,  -4,
     -4,  -3,  -3,  -3,  -3,  -3,  -3,  -3,  -3,  -3,  -3,  -3,
     -3,  -3,  -3,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,
     -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,
     -1,  -1,  -1,  -1,  -1,  -1,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0
};

#define UPDATE_LEAK()           \
do {                            \
    fastleak += fdecay;         \
    if (fastleak > psd + fgain) \
        fastleak = psd + fgain; \
    slowleak += sdecay;         \
    if (slowleak > psd + sgain) \
        slowleak = psd + sgain; \
} while (0)

#define COMPUTE_MASK()                          \
do {                                            \
    if (psd > dbknee)                           \
        mask -= (psd - dbknee) >> 2;            \
    if (mask > hth [i >> halfrate])             \
        mask = hth [i >> halfrate];             \
    mask -= snroffset + 128 * deltba[i];        \
    mask = (mask > 0) ? 0 : ((-mask) >> 5);     \
    mask -= floor;                              \
} while (0)

const int masktab[256] = 
{  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 
  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 
  20, 21, 22, 23, 24, 25, 26, 27, 28, 28,
  28, 29, 29, 29, 30, 30, 30, 31, 31, 31,
  32, 32, 32, 33, 33, 33, 34, 34, 34, 35,
  35, 35, 35, 35, 35, 36, 36, 36, 36, 36,
  36, 37, 37, 37, 37, 37, 37, 38, 38, 38,
  38, 38, 38, 39, 39, 39, 39, 39, 39, 40,
  40, 40, 40, 40, 40, 41, 41, 41, 41, 41, 
  41, 41, 41, 41, 41, 41, 41, 42, 42, 42,
  42, 42, 42, 42, 42, 42, 42, 42, 42, 43, 
  43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 
  43, 44, 44, 44, 44, 44, 44, 44, 44, 44,
  44, 44, 44, 45, 45, 45, 45, 45, 45, 45,
  45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 
  45, 45, 45, 45, 45, 45, 45, 46, 46, 46, 
  46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 
  46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 
  46, 47, 47, 47, 47, 47, 47, 47, 47, 47,
  47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 
  47, 47, 47, 47, 47, 48, 48, 48, 48, 48, 
  48, 48, 48, 48, 48, 48, 48, 48, 48, 48,
  48, 48, 48, 48, 48, 48, 48, 48, 48, 49,
  49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 
  49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 
  49, 49, 49, 00, 00, 00 };



void 
Parser::bit_allocate (chinfo_t &chinfo)
{
  const int slowgain[4] = {0x540, 0x4d8, 0x478, 0x410};
  const int dbpbtab[4]  = {0xc00, 0x500, 0x300, 0x100};
  const int floortab[8] = {0x910, 0x950, 0x990, 0x9d0,
                           0xa10, 0xa90, 0xb10, 0x1400};

  int i, j;

  int fdecay, fgain, sdecay, sgain, dbknee, floor, snroffset;
  int psd, mask;
  const int8_t * deltba;
  const int * hth;


  fdecay = (63 + 20 * ((bai >> 7) & 3)) >> halfrate;       // fdcycod
  fgain = chinfo.fgain;
//  fgain = 128 + 128 * (ba->bai & 7);                     // fgaincod
  sdecay = (15 + 2 * (bai >> 9)) >> halfrate;              // sdcycod
  sgain = slowgain[(bai >> 5) & 3];                        // sgaincod
  dbknee = dbpbtab[(bai >> 3) & 3];                        // dbpbcod
  hth = hthtab[fscod];
  /*
   * if there is no delta bit allocation, make deltba point to an area
   * known to contain zeroes. baptab+156 here.
   */
  deltba = (chinfo.deltbae == DELTA_BIT_NONE) ? baptab + 156 : chinfo.deltba;
  floor = floortab[bai & 7];                               // floorcod
  snroffset = 960 - 64 * csnroffst - 4 * chinfo.fsnroffst + floor;
  floor >>= 5;

  int start = chinfo.strtmant;
  int end = chinfo.endmant;
  int fastleak = chinfo.fastleak;
  int slowleak = chinfo.slowleak;


  int8_t *bap = chinfo.bap;
  exp_t  *exp = chinfo.exp;


  i = masktab[chinfo.strtmant];
  j = start;
  if (start == 0) // not the coupling channel 
  {   
    int lowcomp;

    lowcomp = 0;
    j = end - 1;
    do 
    {
      if (i < j) 
      {
        if (exp[i+1] == exp[i] - 2)
          lowcomp = 384;
        else if (lowcomp && (exp[i+1] > exp[i]))
          lowcomp -= 64;
      }
      psd = 128 * exp[i];
      mask = psd + fgain + lowcomp;
      COMPUTE_MASK ();
      bap[i] = (baptab+156)[mask + 4 * exp[i]];
      i++;
    } while ((i < 3) || ((i < 7) && (exp[i] > exp[i-1])));
    fastleak = psd + fgain;
    slowleak = psd + sgain;

    while (i < 7) 
    {
      if (i < j) 
      {
        if (exp[i+1] == exp[i] - 2)
          lowcomp = 384;
        else if (lowcomp && (exp[i+1] > exp[i]))
          lowcomp -= 64;
      }
      psd = 128 * exp[i];
      UPDATE_LEAK ();
      mask = ((fastleak + lowcomp < slowleak) ?
              fastleak + lowcomp : slowleak);
      COMPUTE_MASK ();
      bap[i] = (baptab+156)[mask + 4 * exp[i]];
      i++;
    }

    if (end == 7)   /* lfe channel */
      return;

    do 
    {
      if (exp[i+1] == exp[i] - 2)
        lowcomp = 320;
      else if (lowcomp && (exp[i+1] > exp[i]))
        lowcomp -= 64;
      psd = 128 * exp[i];
      UPDATE_LEAK ();
      mask = ((fastleak + lowcomp < slowleak) ?
              fastleak + lowcomp : slowleak);
      COMPUTE_MASK ();
      bap[i] = (baptab+156)[mask + 4 * exp[i]];
      i++;
    } while (i < 20);

    while (lowcomp > 128) /* two iterations maximum */
    {         
      lowcomp -= 128;
      psd = 128 * exp[i];
      UPDATE_LEAK ();
      mask = ((fastleak + lowcomp < slowleak) ?
              fastleak + lowcomp : slowleak);
      COMPUTE_MASK ();
      bap[i] = (baptab+156)[mask + 4 * exp[i]];
      i++;
    }
    j = i;
  }

  do 
  {
    int startband, endband;

    startband = j;
    endband = ((bndtab-20)[i] < end) ? (bndtab-20)[i] : end;
    psd = 128 * exp[j++];
    while (j < endband) 
    {
      int next, delta;

      next = 128 * exp[j++];
      delta = next - psd;
      switch (delta >> 9) 
      {
        case -6: case -5: case -4: case -3: case -2:
          psd = next;
          break;
        case -1:
          psd = next + latab[(-delta) >> 1];
          break;
        case 0:
          psd += latab[delta >> 1];
          break;
      }
    }
    /* minpsd = -289 */
    UPDATE_LEAK ();
    mask = (fastleak < slowleak) ? fastleak : slowleak;
    COMPUTE_MASK ();
    i++;
    j = startband;
    do 
    {
        /* max(mask+4*exp)=147=-(minpsd+fgain-deltba-snroffset)>>5+4*exp */
        /* min(mask+4*exp)=-56=-(sgain-deltba-snroffset)>>5 */
        bap[j] = (baptab+156)[mask + 4 * exp[j]];
    } while (++j < endband);
  } while (j < end);
}
