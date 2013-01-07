/* 다른 파일로 만든 함수가 있으면 C컴파일러에게 알려줌 */

void io_hlt(void);

void HariMain(void)
{
  int i;
  char *p = (char *) 0xa0000;	/* 번지를 대입 */

  for (i = 0; i <= 0xffff; i++) {
    p[i] = i & 0x0f;
  }

  for (;;) {
    io_hlt();  /* 이것으로 naskfunc.nas의 _io_hlt가 실행된다. */
  }
}
