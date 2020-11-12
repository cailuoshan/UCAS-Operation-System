#include <os/list.h>
#include <os/mm.h>
#include <os/lock.h>
#include <os/sched.h>
#include <os/time.h>
#include <os/irq.h>
#include <screen.h>
#include <stdio.h>
#include <assert.h>

pcb_t pcb[NUM_MAX_TASK];
const ptr_t pid0_stack = INIT_KERNEL_STACK + PAGE_SIZE;
pcb_t pid0_pcb = {
    .pid = 0,
    .kernel_sp = (ptr_t)pid0_stack,
    .user_sp = (ptr_t)pid0_stack,
    .preempt_count = 0
};  //代表内核本身

LIST_HEAD(ready_queue);

/* current running task PCB */
pcb_t * volatile current_running;

/* global process id */
pid_t process_id = 1;

//task 1
void do_scheduler(void)
{
    __asm__ __volatile__("csrr x0, sscratch\n");  
    // TODO schedule
    // Modify the current_running pointer.
    pcb_t *prev_running;
    prev_running = current_running;
    /* no priority
    if(current_running->status!=TASK_BLOCKED){
        current_running->status=TASK_READY;
        if(current_running->pid!=0){
            list_add(&current_running->list,&ready_queue);
        }
    } */
    /* priority scheduler */
    if(current_running->status!=TASK_BLOCKED){
        current_running->status=TASK_READY;
        if(current_running->pid!=0){
            current_running->list.priority--;
            list_add_priority(&current_running->list,&ready_queue);
        }
    }
    
    if(!list_empty(&ready_queue)){
        current_running = list_entry(ready_queue.prev, pcb_t, list);
        list_del(ready_queue.prev);
    }
    current_running->status=TASK_RUNNING;

    // restore the current_runnint's cursor_x and cursor_y
    vt100_move_cursor(current_running->cursor_x,
                      current_running->cursor_y);
    screen_cursor_x = current_running->cursor_x;
    screen_cursor_y = current_running->cursor_y;
    // TODO: switch_to current_running
    switch_to(prev_running, current_running);
}
 
//task 3:将调用该方法的进程挂起到全局阻塞队列，当睡眠时间达到后再由调度器从阻塞队列将其加入到就绪队列中继续运行
void do_sleep(uint32_t sleep_time)
{
    // TODO: sleep(seconds)
    // note: you can assume: 1 second = `timebase` ticks
    // 1. block the current_running
    current_running->status=TASK_BLOCKED;
    // 2. create a timer which calls `do_unblock` when timeout, <time.h>
    timer_create((TimerCallback)(&do_unblock), &current_running->list, sleep_time*time_base);
    // 3. reschedule because the current_running is blocked.
    do_scheduler();
}

//task 2
//add the node into block_queue
void do_block(list_node_t *pcb_node, list_head *queue)
{
    // TODO: block the pcb task into the block queue
    list_add(pcb_node,queue);
    do_scheduler();
}

//move the node from block_queue into ready_queue
void do_unblock(list_node_t *pcb_node)
{
    // TODO: unblock the `pcb` from the block queue
    list_move(pcb_node,&ready_queue);
}

//task4: for lock in user state
int do_binsemget(int key)
{
    int id = key%16;
    return id;
}

int do_binsemop(int binsem_id, int op)
{
    mutex_lock_t *lock = &binsem[binsem_id];
    if(op==0){
        do_mutex_lock_acquire(lock);
    }else if(op==1){
        do_mutex_lock_release(lock);
    }
    return 1;
}
