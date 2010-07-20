
#ifdef _MSC_VER
  #pragma warning (disable: 4244 4146 4081 4018 4996)
#endif

#define FPM_64BIT
#define OPT_ACCURACY

#include "bit.c"
#include "decoder.c"
#include "fixed.c"
#include "frame.c"
#include "huffman.c"
#include "layer12.c"
#include "layer3.c"
#include "stream.c"
#include "synth.c"
#include "timer.c"
#include "version.c"
