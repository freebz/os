; naskfunc
; TAB=4

[FORMAT "WCOFF"]	; 오브젝트 파일을 만드는 모드
[INSTRSET "i486p"]	; 486명령까지 확장
[BITS 32]		; 32비트 모드용의 기계어를 만든다.

; 오브젝트 파일을 위한 정보
[FILE "naskfunc.nas"]  ; 소스 파일명 정보

      GLOBAL	  _io_hlt     ; 이 프로그램에 포함된 함수명

; 이하는 실제의 함수

[SECTION .text]		; 오브젝트 파일에서는 이것을 쓴 후에 프로그램을 쓴다.
_io_hlt: 		; void io_hlt(void);
	HLT
	RET

