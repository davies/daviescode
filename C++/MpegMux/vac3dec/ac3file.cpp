#include "ac3file.h"


AC3FileDecoder::AC3FileDecoder(FILE *_f, Speakers spk):
PullDecoder(spk)
{
  close_f = false;
  f = _f;
}

AC3FileDecoder::AC3FileDecoder(const char *filename, Speakers spk):
PullDecoder(spk)
{
  close_f = true;
  f = 0;
  f = fopen(filename, "rb");
}

AC3FileDecoder::~AC3FileDecoder()
{
  if (f && close_f)
    fclose(f);
}

void AC3FileDecoder::init()
{
	reset();
	mixer.out_mode = Speakers(MODE_STEREO);
	mixer.level = 1.0;
	
//	eq.on = true;
//	d.set_on(true);
//	int values[6] = {2000, 0, 2000, 0, 0, 0 };
//	d.set_delay(values);
}

long
AC3FileDecoder::get(uint8_t *buf, int len)
{
  if (!f) return 0;

  return fread(buf, 1, len, f);
}
