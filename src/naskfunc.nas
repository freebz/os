; naskfunc
; TAB=4

[FORMAT "WCOFF"]	; ������Ʈ ������ ����� ���
[INSTRSET "i486p"]	; 486��ɱ��� Ȯ��
[BITS 32]		; 32��Ʈ ������ ��� �����.

; ������Ʈ ������ ���� ����
[FILE "naskfunc.nas"]  ; �ҽ� ���ϸ� ����

      GLOBAL	  _io_hlt, _write_mem8     ; �� ���α׷��� ���Ե� �Լ���

; ���ϴ� ������ �Լ�

[SECTION .text]		; ������Ʈ ���Ͽ����� �̰��� �� �Ŀ� ���α׷��� ����.
_io_hlt: 		; void io_hlt(void);
	HLT
	RET

_write_mem8:		; void write_mem8(int addr, int data);
	MOV	ECX, [ESP+4]   ; [ESP+4]�� addr�� ��� �����Ƿ� �װ��� ECX�� �о����
	MOV	AL, [ESP+8]    ;
	MOV	[ECX], AL
	RET
