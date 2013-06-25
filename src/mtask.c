/* mtask.c */

#include "bootpack.h"

struct TIMER *mt_timer;
int mt_tr;

void mt_init(void)
{
  mt_timer = timer_alloc();
  /* timer_init은 필요가 없기 때문에 하지 않는다. */
  timer_settime(mt_timer, 2);
  mt_tr = 3 * 8;
  return;
}

void mt_taskswitch(void)
{
  if (mt_tr == 3 * 8) {
    mt_tr = 4 * 8;
  } else {
    mt_tr = 3 * 8;
  }
  timer_settime(mt_timer, 2);
  farjmp(0, mt_tr);
  return;
}
