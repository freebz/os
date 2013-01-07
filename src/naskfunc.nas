; naskfunc
; TAB=4

[FORMAT "WCOFF"]	; 오브젝트 파일을 만드는 모드
[INSTRSET "i486p"]	; 486명령까지 확장
[BITS 32]		; 32비트 모드용의 기계어를 만든다.

; 오브젝트 파일을 위한 정보
[FILE "naskfunc.nas"]  ; 소스 파일명 정보

      GLOBAL	  _io_hlt, _write_mem8     ; 이 프로그램에 포함된 함수명

; 이하는 실제의 함수

[SECTION .text]		; 오브젝트 파일에서는 이것을 쓴 후에 프로그램을 쓴다.
_io_hlt: 		; void io_hlt(void);
	HLT
	RET

_write_mem8:		; void write_mem8(int addr, int data);
	MOV	ECX, [ESP+4]   ; [ESP+4]에 addr이 들어 있으므로 그것을 ECX에 읽어들임
	MOV	AL, [ESP+8]    ;
	MOV	[ECX], AL
	RET
