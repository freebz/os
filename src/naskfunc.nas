; naskfunc
; TAB=4

[FORMAT "WCOFF"]	; ������Ʈ ������ ����� ���
[INSTRSET "i486p"]	; 486��ɱ��� Ȯ��
[BITS 32]		; 32��Ʈ ������ ��� �����.

; ������Ʈ ������ ���� ����
[FILE "naskfunc.nas"]  ; �ҽ� ���ϸ� ����

      GLOBAL	  _io_hlt     ; �� ���α׷��� ���Ե� �Լ���

; ���ϴ� ������ �Լ�

[SECTION .text]		; ������Ʈ ���Ͽ����� �̰��� �� �Ŀ� ���α׷��� ����.
_io_hlt: 		; void io_hlt(void);
	HLT
	RET

