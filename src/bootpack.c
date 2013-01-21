#include <stdio.h>
#include "bootpack.h"

extern struct FIFO8 keyfifo;

void init_keyboard(void);
void enable_mouse(void);

void HariMain(void)
{
  struct BOOTINFO *binfo = (struct BOOTINFO *) 0x0ff0;
  char s[40], mcursor[256], keybuf[32];
  int mx, my;
  int i, j;

  init_palette();
  init_screen(binfo->vram, binfo->scrnx, binfo->scrny);

  putfonts8_asc(binfo->vram, binfo->scrnx,  8,  8, COL8_FFFFFF, "ABC 123");
  putfonts8_asc(binfo->vram, binfo->scrnx, 31, 31, COL8_000000, "Haribote OS.");
  putfonts8_asc(binfo->vram, binfo->scrnx, 30, 30, COL8_FFFFFF, "Haribote OS.");

  sprintf(s, "scrnx = %d", binfo->scrnx);
  putfonts8_asc(binfo->vram, binfo->scrnx, 16, 64, COL8_FFFFFF, s);

  mx = (binfo->scrnx - 16) / 2;
  my = (binfo->scrny - 28 - 16) / 2;
  init_mouse_cursor8(mcursor, COL8_008484);
  putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
  
  init_gdtidt();
  init_pic();
  io_sti();	/* IDT/PIC�� �ʱ�ȭ�� �������Ƿ� CPU�� ���ͷ�Ʈ ������ ���� */

  io_out8(PIC0_IMR, 0xf9);	/* PIC1�� Ű���带 �㰡 (11111001) */
  io_out8(PIC1_IMR, 0xef);	/* ���콺�� �㰡 (11101111) */
  
  fifo8_init(&keyfifo, 32, keybuf);

  init_keyboard();
  enable_mouse();
 
  for (;;) {
    io_cli();
    if (fifo8_status(&keyfifo) == 0) {
      io_stihlt();
    } else {
      i = fifo8_get(&keyfifo);
      io_sti();
      sprintf(s, "%02X", i);
      boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 16, 15, 31);
      putfonts8_asc(binfo->vram, binfo->scrnx, 0, 16, COL8_FFFFFF, s);
    }
  }
}

#define PORT_KEYDAT	0x0060
#define PORT_KEYSTA	0x0064
#define PORT_KEYCMD	0x0064
#define KEYSTA_SEND_NOTREADY	0x02
#define KEYCMD_WRITE_MODE	0x60
#define KBC_MODE		0x47

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

#define KEYCMD_SENDTO_MOUSE	0xd4
#define MOUSECMD_ENABLE		0xf4

void enable_mouse(void)
{
  /* ���콺 ��ȿ */
  wait_KBC_sendready();
  io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
  wait_KBC_sendready();
  io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
  return;	/* �� �Ǹ� ACK(0xfa)�� �۵ȵǾ� �´�. */
}
