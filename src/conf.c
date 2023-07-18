#include "conf.h"
unsigned int roundUpToPowerOf2(unsigned int base)
{
  base -= 1;

  base = base | (base >> 1);
  base = base | (base >> 2);
  base = base | (base >> 4);
  base = base | (base >> 8);
  base = base | (base >> 16);

  return base + 1;
}