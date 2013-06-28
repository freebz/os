/* mtask.c */

#include "bootpack.h"

struct TASKCTL *taskctl;
struct TIMER *task_timer;

struct TASK *task_init(struct MEMMAN *memman)
{
  int i;
  struct TASK *task;
  struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) ADR_GDT;
  taskctl = (struct TASKCTL *) memman_alloc_4k(memman, sizeof(struct TASKCTL));

  for (i = 0; i < MAX_TASKS; i++) {
    taskctl->tasks0[i].flags = 0;
    taskctl->tasks0[i].sel = (TASK_GDT0 + i) * 8;
    set_segmdesc(gdt + TASK_GDT0 + i, 103, (int) &taskctl->tasks0[i].tss, AR_TSS32);
  }
  task = task_alloc();
  task->flags = 2; /* 동작 중 마크 */
  taskctl->running = 1;
  taskctl->now = 0;
  taskctl->tasks[0] = task;
  load_tr(task->sel);
  task_timer = timer_alloc();
  timer_settime(task_timer, 2);
  return task;
}

struct TASK *task_alloc(void)
{
  int i;
  struct TASK *task;
  for (i = 0; i < MAX_TASKS; i++) {
    if (taskctl->tasks0[i].flags == 0) {
      task = &taskctl->tasks0[i];
      task->flags = 1; /* 사용 중 마크 */
      task->tss.eflags = 0x00000202; /* IF = 1; */
      task->tss.eax = 0; /* 우선 0으로 해 두기로 한다. */
      task->tss.ecx = 0;
      task->tss.edx = 0;
      task->tss.ebx = 0;
      task->tss.ebp = 0;
      task->tss.esi = 0;
      task->tss.edi = 0;
      task->tss.es = 0;
      task->tss.ds = 0;
      task->tss.fs = 0;
      task->tss.gs = 0;
      task->tss.ldtr = 0;
      task->tss.iomap = 0x40000000;
      return task;
    }
  }
  return 0; /* 벌써 전부 사용 중*/
}

void task_run(struct TASK *task)
{
  task->flags = 2; /* 동작 중 마크 */
  taskctl->tasks[taskctl->running] = task;
  taskctl->running++;
  return;
}

void task_switch(void)
{
  timer_settime(task_timer, 2);
  if (taskctl->running >= 2) {
    taskctl->now++;
    if (taskctl->now == taskctl->running) {
      taskctl->now = 0;
    }
    farjmp(0, taskctl->tasks[taskctl->now]->sel);
  }
  return;
}

void task_sleep(struct TASK *task)
{
  int i;
  char ts = 0;
  if (task->flags == 2) { /* 지정한 태스크가 만약 깨어 있으면 */
    if (task == taskctl->tasks[taskctl->now]) {
      ts = 1; /* 자기 자신을 재우므로, 나중에 태스크를 스위치한다. */
    }
    /* task가 어디에 있는지 찾는다. */
    for (i = 0; i < taskctl->running; i++) {
      if (taskctl->tasks[i] == task) {
	/* 여기에 있었다. */
	break;
      }
    }
    taskctl->running--;
    if (i < taskctl->now) {
      taskctl->now--; /* 어긋나 있으므로, 이것도 맞추어 둔다. */
    }
    /* 옮겨 놓기 */
    for (; i < taskctl->running; i++) {
      taskctl->tasks[i] = taskctl->tasks[i + 1];
    }
    task->flags = 1; /* 동작하고 있지 않은 상태 */
    if (ts != 0) {
      /* 태스크를 스위치한다. */
      if (taskctl->now >= taskctl->running) {
	/* now가 이상한 값이 되어 있으면, 수정한다. */
	taskctl->now = 0;
      }
      farjmp(0, taskctl->tasks[taskctl->now]->sel);
    }
  }
  return;
}

