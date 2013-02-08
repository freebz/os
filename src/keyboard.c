/* keyboad.c */

#include "bootpack.h"

struct FIFO8 keyfifo;

void wait_KBC_sendready(void)
{
  /* 키보드 컨트롤러가 데이터 송신을 가능하게 해 주는 것을 기다린다. */
  for (;;) {
    if ((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0) {
      break;
    }
  }
  return;
}

void init_keyboard(void)
{
  /* 키보드 컨트롤러의 초기화 */
  wait_KBC_sendready();
  io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
  wait_KBC_sendready();
  io_out8(PORT_KEYDAT, KBC_MODE);
  return;
}

void inthandler21(int *esp)
/* PS/2 키보드로부터의 인터럽트 */
{
  unsigned char data;
  io_out8(PIC0_OCW2, 0x61);	/* IRQ-01 접수 완료를 PIC에 통지 */
  data = io_in8(PORT_KEYDAT);
  fifo8_put(&keyfifo, data);
  return;
}
