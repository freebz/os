;hello-os
;TAB=4

	ORG	0x7c00		; �޸� �� ��� �ε��Ǵ°�
				; ��Ʈ���Ͱ� �������� ��巹��

;���ϴ� ǥ������ FAT12 ���� �÷��ǵ�ũ�� ���� ����

	JMP	 entry
	DB	 0x90
	DB	 "HelloIPL"  ;��Ʈ������ �̸��� �����Ӱ� �ᵵ ����
	DW	 512	     ;1������ ũ��(����Ʈ ����.512�� �ؾ� ��)
	DB	 1	     ;Ŭ�������� ũ��(1���ͷ� �ؾ� ��)
	DW	 1	     ;����� ������ ��
	DB	 2	     ;��ũ�� FAT ���̺��� ��
	DW	 224	     ;��Ʈ ���丮 ��Ʈ���� ��(������ 224��Ʈ��)
	DW	 2880	     ;��ũ�� �� ���� ��(2880���ͷ� �ؾ� ��)
	DB	 0xf0	     ;�̵�� Ÿ��(0xf0���� �ؾ���)
	DW	 9	     ;�ϳ��� FAT ���̺��� ���� ��(9���ͷ� �ؾ� ��)
	DW	 18	     ;1Ʈ���� �� ���Ͱ� �ִ°�(18�� �ؾ� ��)
	DW	 2	     ;����� ��(2�� �ؾ� ��)
	DD	 0	     ;��Ƽ���� ������� �����Ƿ� �̰��� �ݵ�� 0
	DD	 2880	     ;�� ����̺��� ũ�⸦ �� �� �� ��
	DB	 0, 0, 0x29  ;�� �� �� ���� �־� �θ� ����
	DD	 0xffffffff  ;�Ƹ� ���� �ø��� ��ȣ
	DB	 "HELLO-OS  "	   ;��ũ �̸�
	DB	 "FAT12   " ;������ �̸�(8����Ʈ)
	RESB	 18	  ; 18����Ʈ ���ܵ�

;���α׷� ��ü

entry:
	MOV	AX, 0		; �������� �ʱ�ȭ
	MOV	SS, AX
	MOV	SP, 0x7c00
	MOV	DS, AX

;��ũ�� �д´�.

	MOV	AX, 0x0820
	MOV	ES, AX
	MOV	CH, 0		; �Ǹ��� 0
	MOV	DH, 0		; ��� 0
	MOV	CL, 2		; ���� 2

readloop:
	MOV	SI, 0		; ���� Ƚ���� ���� ��������
retry:
	MOV	AH, 0x02	; AH=0x02 : ��ũ read
	MOV	AL, 1		; 1����
	MOV	BX, 0
	MOV	DL, 0x00	; A����̺�
	INT	0x13		; ��ũ BIOS ȣ��
	JNC	next		; ������ ������ next����
	ADD	SI, 1		; SI�� 1�� ���Ѵ�.
	CMP	SI, 5		; SI�� 5�� ��
	JAE	error		; SI >= 5 �̸� error��
	MOV	AH, 0x00
	MOV	DL, 0x00	; A����̺�
	INT	0x13		; ����̺� ����
	JMP	retry

next:
	MOV	AX, ES		; ��巹���� 0x200 ���Ѵ�. 512/16
	ADD	AX, 0x0020
	MOV	ES, AX		; ES�� �����ϱ� ���ؼ� AX�� �̿�
	ADD	CL, 1		; CL�� 1�� ���Ѵ�.
	CMP	CL, 18		; CL�� 18�� ��
	JBE	readloop	; CL <= 18�̶�� readloop��

;�� �о����� �켱 ������ ���� ������ sleep

fin:
	HLT			; CPU�� ������Ŵ
	JMP	fin		; ���ѷ���

error:
	MOV	AX, 0
	MOV	ES, AX
	MOV	SI, msg

putloop:
	MOV	AL, [SI]
	ADD	SI, 1		; SI�� 1�� ���Ѵ�.
	CMP	AL, 0
	JE	fin
	MOV	AH, 0x0e	; �� ���� ǥ�� ���
	MOV	BX, 15		; �÷� �ڵ�
	INT	0x10		; ���� BIOS ȣ��
	JMP	putloop

;�޽��� �κ�
msg:
	DB	0x0a, 0x0a	; �� �ٲ� 2��
	DB	"load error"
	DB	0x0a		; �� �ٲ� 1��
	DB	0

	RESB	0x7dfe-$

	DB	0x55, 0xaa
