/* keyboad.c */

#include "bootpack.h"

struct FIFO8 keyfifo;

void wait_KBC_sendready(void)
{
  /* Ű���� ��Ʈ�ѷ��� ������ �۽��� �����ϰ� �� �ִ� ���� ��ٸ���. */
  for (;;) {
    if ((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0) {
      break;
    }
  }
  return;
}

void init_keyboard(void)
{
  /* Ű���� ��Ʈ�ѷ��� �ʱ�ȭ */
  wait_KBC_sendready();
  io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
  wait_KBC_sendready();
  io_out8(PORT_KEYDAT, KBC_MODE);
  return;
}

void inthandler21(int *esp)
/* PS/2 Ű����κ����� ���ͷ�Ʈ */
{
  unsigned char data;
  io_out8(PIC0_OCW2, 0x61);	/* IRQ-01 ���� �ϷḦ PIC�� ���� */
  data = io_in8(PORT_KEYDAT);
  fifo8_put(&keyfifo, data);
  return;
}
