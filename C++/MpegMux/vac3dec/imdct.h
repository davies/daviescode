#ifndef IMDCT_H
#define IMDCT_H

#include "defs.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795029
#endif



typedef struct complex_s {
    sample_t real;
    sample_t imag;
} complex_t;

class IMDCT
{
protected:
  // Root values for IFFT
  sample_t roots16[3];
  sample_t roots32[7];
  sample_t roots64[15];
  sample_t roots128[31];

  // Twiddle factors for IMDCT
  complex_t pre1[128];
  complex_t post1[64];
  complex_t pre2[64];
  complex_t post2[32];

  // IFFT functions
  void ifft_pass (complex_t * buf, sample_t * weight, int n);
  inline void ifft2 (complex_t * buf);
  inline void ifft4 (complex_t * buf);
  void ifft8 (complex_t * buf);
  void ifft16  (complex_t * buf);
  void ifft32  (complex_t * buf);
  void ifft64  (complex_t * buf);
  void ifft128 (complex_t * buf);


  complex_t buf[128];
public:
  IMDCT();

  void imdct_512 (sample_t *data, sample_t *delay);
  void imdct_256 (sample_t *data, sample_t *delay);
};


inline void 
IMDCT::ifft2 (complex_t * buf)
{
  double r, i;

  r = buf[0].real;
  i = buf[0].imag;
  buf[0].real += buf[1].real;
  buf[0].imag += buf[1].imag;
  buf[1].real = r - buf[1].real;
  buf[1].imag = i - buf[1].imag;
}

inline void 
IMDCT::ifft4 (complex_t * buf)
{
  double tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;

  tmp1 = buf[0].real + buf[1].real;
  tmp2 = buf[3].real + buf[2].real;
  tmp3 = buf[0].imag + buf[1].imag;
  tmp4 = buf[2].imag + buf[3].imag;
  tmp5 = buf[0].real - buf[1].real;
  tmp6 = buf[0].imag - buf[1].imag;
  tmp7 = buf[2].imag - buf[3].imag;
  tmp8 = buf[3].real - buf[2].real;

  buf[0].real = tmp1 + tmp2;
  buf[0].imag = tmp3 + tmp4;
  buf[2].real = tmp1 - tmp2;
  buf[2].imag = tmp3 - tmp4;
  buf[1].real = tmp5 + tmp7;
  buf[1].imag = tmp6 + tmp8;
  buf[3].real = tmp5 - tmp7;
  buf[3].imag = tmp6 - tmp8;
}


#endif