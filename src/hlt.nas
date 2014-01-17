[BITS 32]
      MOV	AL, 'A'
      CALL	2*8:0xba5
fin:
	HLT
	JMP fin
