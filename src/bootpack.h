/* asmhead.nas */
struct BOOTINFO { /* 0x0ff0-0x0fff */
  char cyls;	  /* 부트섹터는 어디까지 디스크를 읽을 것인가 */
  char leds;	  /* 부팅 시 키보드의 LED 상태 */
  char vmode;	  /* 비디오 모드 몇 비트 컬러인가 */
  char reserve;
  short scrnx, scrny;	/* 화면 해상도 */
  char *vram;
};
#define ADR_BOOTINFO 0x00000ff0

/* naskfunc.nas */
void io_hlt(void);
void io_cli(void);
void io_sti(void);
void io_stihlt(void);
int io_in8(int port);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);
void asm_inthandler21(void);
void asm_inthandler2c(void);
int load_cr0(void);
void store_cr0(int cr0);
unsigned int memtest_sub(unsigned int start, unsigned int end);
void asm_inthandler20(void);
void farjmp(int eip, int cs);

/* graphic.c */
void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *, int, unsigned char, int, int, int, int);
void init_screen(char *vram, int xsize, int ysize);
void putfont8(char *vram, int xsize, int x, int y, char c, char *font);
void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s);
void init_mouse_cursor8(char *mouse, char bc);
void putblock8_8(char *vram, int vxsize, int pxsize,
		 int pysize, int px0, int py0, char *buf, int bxsize);

#define COL8_000000 0
#define COL8_FF0000 1
#define COL8_00FF00 2
#define COL8_FFFF00 3
#define COL8_0000FF 4
#define COL8_FF00FF 5
#define COL8_00FFFF 6
#define COL8_FFFFFF 7
#define COL8_C6C6C6 8
#define COL8_840000 9
#define COL8_008400 10
#define COL8_848400 11
#define COL8_000084 12
#define COL8_840084 13
#define COL8_008484 14
#define COL8_848484 15

/* dsctbl.c */
struct SEGMENT_DESCRIPTOR {
  short limit_low, base_low;
  char base_mid, access_right;
  char limit_high, base_high;
};

struct GATE_DESCRIPTOR {
  short offset_low, selector;
  char dw_count, access_right;
  short offset_high;
};

void init_gdtidt(void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);

#define ADR_IDT		0x0026F800
#define LIMIT_IDT	0x000007ff
#define ADR_GDT		0x00270000
#define LIMIT_GDT	0x0000ffff
#define ADR_BOTPAK	0x00280000
#define LIMIT_BOTPAK	0x0007ffff
#define AR_DATA32_RW	0x4092
#define AR_CODE32_ER	0x409a
#define AR_TSS32	0x0089
#define AR_INTGATE32	0x008e	/* IDT에 대한 속성 설정 : 인터럽트 처리용의 유효한 설정을 나타내는 마크 */

/* int.c */
void init_pic(void);

#define PIC0_ICW1	0x0020
#define PIC0_OCW2	0x0020
#define PIC0_IMR	0x0021
#define PIC0_ICW2	0x0021
#define PIC0_ICW3	0x0021
#define PIC0_ICW4	0x0021
#define PIC1_ICW1	0x00a0
#define PIC1_OCW2	0x00a0
#define PIC1_IMR	0x00a1
#define PIC1_ICW2	0x00a1
#define PIC1_ICW3	0x00a1
#define PIC1_ICW4	0x00a1



/* fifo.c */
struct FIFO8 {
  unsigned char *buf;
  int p, q, size, free, flags;
};

struct FIFO32 {
  int *buf;
  int p, q, size, free, flags;
  struct TASK *task;
};

void fifo8_init(struct FIFO8 *fifo, int size, unsigned char *buf);
int fifo8_put(struct FIFO8 *fifo, unsigned char data);
int fifo8_get(struct FIFO8 *fifo);
int fifo8_status(struct FIFO8 *fifo);
void fifo32_init(struct FIFO32 *fifo, int size, int *buf, struct TASK *task);
int fifo32_put(struct FIFO32 *fifo, int data);
int fifo32_get(struct FIFO32 *fifo);
int fifo32_status(struct FIFO32 *fifo);


/* keyboard.c */
void inthandler21(int *esp);
void wait_KBC_sendready(void);
void init_keyboard(struct FIFO32 *fifo, int data0);

#define PORT_KEYDAT		0x0060
#define PORT_KEYCMD		0x0064

/* mouse.c */
struct MOUSE_DEC {
  unsigned char buf[3], phase;
  int x, y, btn;
};

void inthandler2c(int *esp);
void enable_mouse(struct FIFO32 *fifo, int data0, struct MOUSE_DEC *mdec);
int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat);


#define KEYCMD_SENDTO_MOUSE	0xd4
#define MOUSECMD_ENABLE		0xf4

/* memory.c */
#define MEMMAN_ADDR	0x003c0000
#define MEMMAN_FREES	4090	/* 이것으로 약 32KB 정도의 관리 영역이 필요 */

struct FREEINFO {	/* 빈 정보 */
  unsigned int addr, size;
};

struct MEMMAN {		/* 메모리 관리 */
  int frees, maxfrees, lostsize, losts;
  struct FREEINFO free[MEMMAN_FREES];
};

unsigned int memtest(unsigned int start, unsigned int end);
void memman_init(struct MEMMAN *man);
unsigned int memman_total(struct MEMMAN *man);
unsigned int memman_alloc(struct MEMMAN *man, unsigned int size);
int memman_free(struct MEMMAN *man, unsigned int addr, unsigned int size);
unsigned int memman_alloc_4k(struct MEMMAN *man, unsigned int size);
int memman_free_4k(struct MEMMAN *man, unsigned int addr, unsigned int size);

/* sheet.c */
#define MAX_SHEETS	256

struct SHEET {
  unsigned char *buf;
  int bxsize, bysize, vx0, vy0, col_inv, height, flags;
  struct SHTCTL *ctl;
};

struct SHTCTL {
  unsigned char *vram, *map;
  int xsize, ysize, top;
  struct SHEET *sheets[MAX_SHEETS];
  struct SHEET sheets0[MAX_SHEETS];
};

struct SHTCTL *shtclt_init(struct MEMMAN *memman, unsigned char *vram,
			     int xsize, int ysize);
struct SHEET *sheet_alloc(struct SHTCTL *ctl);
void sheet_setbuf(struct SHEET *sht, unsigned char *buf,
		    int xsize, int ysize, int col_inv);
void sheet_updown(struct SHEET *sht, int height);
void sheet_refresh(struct SHEET *sht, int bx0, int bx1, int by0, int by1);
void sheet_refreshsub(struct SHTCTL *ctl, int bx0, int bx1, int by0, int by1, int h0, int h1);
void sheet_slide(struct SHEET *sht, int vx0, int vy0);
void sheet_free(struct SHEET *sht);
void sheet_refreshmap(struct SHTCTL *ctl, int vx0, int vy0, int vx1, int vy1, int h0);

/* timer.c */
void init_pit(void);
struct TIMER *timer_alloc(void);
void timer_free(struct TIMER *timer);
void timer_init(struct TIMER *timer, struct FIFO32 *fifo, unsigned char data);
void timer_settime(struct TIMER *timer, unsigned int timeout);
void inthandler20(int *esp);


#define MAX_TIMER	500

struct TIMER {
  struct TIMER *next;
  unsigned int timeout, flags;
  struct FIFO32 *fifo;
  int data;
};

struct TIMERCTL {
  unsigned int count, next, using;
  struct TIMER *t0;
  struct TIMER timers0[MAX_TIMER];
};

extern struct TIMERCTL timerctl;

/* mtask.c */
#define MAX_TASKS	1000	/* 최대 태스크의 수 */
#define TASK_GDT0	3	/* TSS를 GDT의 몇 번부터 할당하는가 */

struct TSS32 {
  int backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
  int eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
  int es, cs, ss, ds, fs, gs;
  int ldtr, iomap;
};

struct TASK {
  int sel, flags; /* sel은 GDT의 번호 */
  struct TSS32 tss;
};

struct TASKCTL {
  int running; /* 동작하고 있는 태스크 수 */
  int now; /* 현재 동작하고 있는 태스크가 어떤 것인지 알 수 있도록 하기 위한 변수 */
  struct TASK *tasks[MAX_TASKS];
  struct TASK tasks0[MAX_TASKS];
};

extern struct TIMER *task_timer;
struct TASK *task_init(struct MEMMAN *memman);
struct TASK *task_alloc(void);
void task_run(struct TASK *task);
void task_switch(void);
void task_sleep(struct TASK *task);
