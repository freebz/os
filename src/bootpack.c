#include <stdio.h>
#include "bootpack.h"

extern struct KEYBUF keybuf;

void HariMain(void)
{
  struct BOOTINFO *binfo = (struct BOOTINFO *) 0x0ff0;
  char s[40], mcursor[256];
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
  io_sti();	/* IDT/PIC의 초기화가 끝났으므로 CPU의 인터럽트 금지를 해제 */

  io_out8(PIC0_IMR, 0xf9);	/* PIC1와 키보드를 허가 (11111001) */
  io_out8(PIC1_IMR, 0xef);	/* 마우스를 허가 (11101111) */

  for (;;) {
    io_cli();
    if (keybuf.len == 0) {
      io_stihlt();
    } else {
      i = keybuf.data[keybuf.next_r];
      keybuf.len--;
      keybuf.next_r++;
      if (keybuf.next_r == 32) {
	keybuf.next_r = 0;
      }
      io_sti();
      sprintf(s, "%02X", i);
      boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 16, 15, 31);
      putfonts8_asc(binfo->vram, binfo->scrnx, 0, 16, COL8_FFFFFF, s);
    }
  }
}
