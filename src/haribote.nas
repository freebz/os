; haribote-os
; TAB=4

	ORG	0xc200		; �� ���α׷��� ��� �ε��Ǵ°�

	MOV	AL, 0x13	; VGA �׷��Ƚ�, 320x200x8bit �÷�
	MOV	AH, 0x00
	INT	0x10
fin:
	HLT
	JMP fin