/* 다른 파일로 만든 함수가 있으면 C컴파일러에게 알려줌 */

void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);

void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);

void HariMain(void)
{
  int i;
  char *p = (char *) 0xa0000;	/* 번지를 대입 */

  init_palette();
  
  for (i = 0; i <= 0xffff; i++) {
    p[i] = i & 0x0f;
  }

  for (;;) {
    io_hlt();  /* 이것으로 naskfunc.nas의 _io_hlt가 실행된다. */
  }
}

void init_palette(void)
{
  static unsigned char table_rgb[16 * 3] = {
    0x00, 0x00, 0x00,	/* 0: 검은 색 */
    0xff, 0x00, 0x00,	/* 1: 밝은 적색 */
    0x00, 0xff, 0x00,	/* 2: 밝은 녹색 */
    0xff, 0xff, 0x00, 	/* 3: 밝은 노란색 */
    0x00, 0x00, 0xff,	/* 4: 밝은 청색 */
    0xff, 0x00, 0xff,	/* 5: 밝은 보라색 */
    0x00, 0xff, 0xff,	/* 6: 밝은 청색 */
    0xff, 0xff, 0xff,	/* 7: 흰색 */
    0xc6, 0xc6, 0xc6,	/* 8: 밝은 회색 */
    0x84, 0x00, 0x00,	/* 9: 어두운 적색 */
    0x00, 0x84, 0x00,	/* 10: 어두운 녹색 */
    0x84, 0x84, 0x00,	/* 11: 어두운 노란색 */
    0x00, 0x00, 0x84,	/* 12: 군청색 */
    0x84, 0x00, 0x84,	/* 13: 어두운 보라색 */
    0x00, 0x84, 0x84,	/* 14: 어두운 청색 */
    0x84, 0x84, 0x84	/* 15: 어두운 회색 */
  };
  set_palette(0, 15, table_rgb);
  return;
}

void set_palette(int start, int end, unsigned char *rgb)
{
  int i, eflags;
  eflags = io_load_eflags();
  io_cli();
  io_out8(0x03c8, start);
  for (i = start; i <= end; i++) {
    io_out8(0x03c9, rgb[0] / 4);
    io_out8(0x03c9, rgb[1] / 4);
    io_out8(0x03c9, rgb[2] / 4);
    rgb += 3;
  }
  io_store_eflags(eflags);
  return;
}
