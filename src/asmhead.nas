; haribote-os
; TAB=4

[INSTRSET "i486p"]

VBEMODE	EQU	0x105		; 1024 x 768 x 8bit 칼라
; (화면 모드 일람)
;	0x100 :  640 x  400 x 8 bit 칼라
;	0x101 :  640 x  480 x 8 bit 칼라
;	0x103 :  800 x  600 x 8 bit 칼라
;	0x105 : 1024 x  768 x 8 bit 칼라
;	0x107 : 1280 x 1024 x 8 bit 칼라

BOTPAK	EQU	0x00280000	; bootpack의 로드 장소
DSKCAC	EQU	0x00100000	; 디스크 캐쉬 프로그램의 장소
DSKCAC0	EQU	0x00008000	; 디스크 캐쉬 프로그램의 장소(리얼모드)

; BOOT_INFO 관계
CYLS	    EQU	0x0ff0		; 부트섹터가 설정한다.
LEDS	    EQU	0x0ff1
VMODE	    EQU 0x0ff2		; 색 수에 관한 정보. 몇 비트 컬러인가
SCRNX	    EQU 0x0ff4		; 해상도 X
SCRNY	    EQU 0x0ff6		; 해상도 Y
VRAM	    EQU 0x0ff8		; 그래픽 버터의 개시 번지

	ORG	0xc200		; 이 프로그램이 어디에 로딩되는가

; VBE 존재 확인
      	MOV	AX, 0x9000
	MOV	ES, AX
	MOV	DI, 0
	MOV	AX, 0x4f00
	INT	0x10
	CMP	AX, 0x004f
	JNE	scrn320

; VBE의 버전 체크
  	MOV	AX, [ES:DI+4]
	CMP	AX, 0x0200
	JB	scrn320		; if (AX < 0x0200) goto scrn320

; 화면 모드 정보를 얻는다.
       	MOV 	CX, VBEMODE
	MOV	AX, 0x4f01
	INT	0x10
	CMP	AX, 0x004f
	JNE	scrn320

; 화면 모드 정보의 확인
        CMP 	BYTE [ES:DI+0x19], 8
	JNE	scrn320
	CMP	BYTE [ES:DI+0x1b], 4
	JNE	scrn320
	MOV	AX, [ES:DI+0x00]
	AND	AX, 0x0080
	JZ	scrn320		; 모드의 속성의 bit7이 0이었으므로 포기한다.

; 화면 모드의 변환
	MOV	BX, VBEMODE+0x4000
	MOV	AX, 0x4f02
	INT	0x10
	MOV	BYTE [VMODE], 8	; 화면 모드를 메모한다.
	MOV	AX, [ES:DI+0x12]
	MOV	[SCRNX], AX
	MOV	AX, [ES:DI+0x14]
	MOV	[SCRNY], AX
	MOV	EAX, [ES:DI+0x28]
	MOV	[VRAM], EAX
	JMP	keystatus

scrn320:
	MOV	AL, 0x13	; VGA 그래픽스, 320x200x8bit칼라
	MOV	AH, 0x00
	INT	0x10
	MOV	BYTE [VMODE], 8	; 화면 모드를 메모한다(C언어가 참조한다)
	MOV	WORD [SCRNX], 320
	MOV	WORD [SCRNY], 200
	MOV	DWORD [VRAM], 0x000a0000


; 키보드의 LED 상태를 BIOS가 알려준다.

keystatus:
  	MOV    AH, 0x02
	INT    0x16		; keyboard BIOS
	MOV    [LEDS], AL

; PIC가 일절의 인터럽트를 받아들이지 않게 한다.
; AT호환기의 사양에서는 PIC의 초기화를 한다면,
; 이것들의 CLI앞에 해 두지 않으면 이따금 행업 한다.
; PIC의 초기화는 나중에 한다.

	MOV    AL, 0xff
	OUT    0x21, AL
	NOP
	OUT	0xa1, AL

	CLI

; CPU로부터 1MB이상의 메모리에 엑세스 할 수 있도록, A20GATE를 설정

	CALL	waitkbdout
	MOV	AL, 0xd1
	OUT	0x64, AL
	CALL	waitkbdout
	MOV	AL, 0xdf	; enable A20
	OUT	0x60, AL
	CALL	waitkbdout

; 프로텍트 모드 이행

[INSTRSET "i486p"]

	  LGDT	[GDTR0]
	  MOV	EAX, CR0
	  AND	EAX, 0x7fffffff
	  OR	EAX, 0x00000001
	  MOV	CR0, EAX
	  JMP	pipelineflush
pipelineflush:
	  MOV	AX, 1*8
	  MOV	DS, AX
	  MOV	ES, AX
	  MOV	FS, AX
	  MOV	GS, AX
	  MOV	SS, AX

;bootpack의 전송
	  MOV	ESI, bootpack
	  MOV	EDI, BOTPAK
	  MOV	ECX, 512*1024/4
	  CALL	memcpy

;디스크 데이터도 본래의 위치에 전송

; 우선은 boot sector로부터

       MOV	ESI, 0x7c00
       MOV	EDI, DSKCAC
       MOV	ECX, 512/4
       CALL	memcpy

       MOV	ESI, DSKCAC0+512
       MOV	EDI, DSKCAC+512
       MOV	ECX, 0
       MOV	CL, BYTE [CYLS]
       IMUL	ECX, 512*18*2/4
       SUB	ECX, 512/4
       CALL	memcpy

; bootpack의 기동
  	   
	   MOV	EBX, BOTPAK
	   MOV	ECX, [EBX+16]
	   ADD	ECX, 3
	   SHR	ECX, 2
	   JZ	skip
	   MOV	ESI, [EBX+20]
	   ADD	ESI, EBX
	   MOV	EDI, [EBX+12]
	   CALL	memcpy
skip:
	MOV	ESP, [EBX+12]
	JMP	DWORD 2*8:0x0000001b

waitkbdout:
	IN	AL, 0x64
	AND	AL, 0x02
	IN	AL, 0x60
	JNZ	waitkbdout
	RET

memcpy:
	MOV	EAX, [ESI]
	ADD	ESI, 4
	MOV	[EDI], EAX
	ADD	EDI, 4
	SUB	ECX, 1
	JNZ	memcpy
	RET

	ALIGNB	16
GDT0:
	RESB	8
	DW	0xffff, 0x0000, 0x9200, 0x00cf
	DW	0xffff, 0x0000, 0x9a28, 0x0047
	DW	0
GDTR0:
	DW	8*3-1
	DD	GDT0

	ALIGNB	16

bootpack:	
