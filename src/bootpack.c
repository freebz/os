/* �ٸ� ���Ϸ� ���� �Լ��� ������ C�����Ϸ����� �˷��� */

void io_hlt(void);

void HariMain(void)
{
 fin:
  io_hlt();  /* �̰����� naskfunc.nas�� _io_hlt�� ����ȴ�. */
  goto fin;
}
