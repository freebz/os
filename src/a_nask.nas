[FORMAT "WCOFF"]	; 오브젝트 파일을 만드는 모드
[INSTRSET "i486p"]	; 486의 명령까지 사용하고 싶다는 의미
[BITS 32] 		; 32비트 모드용의 기계어를 만듦
[FILE "a_nask.nas"]	; 원시 파일명 정보

      GLOBAL	_api_putchar
      GLOBAL	_api_end

[SECTION .text]

_api_putchar:	; void api_putchar(int c);
	MOV	EDX, 1
	MOV	AL, [ESP+4]	; c
	INT	0x40
	RET

_api_end:	; void api_end(void_;
	MOV	EDX, 4
	INT	0x40
