#include <stdatomic.h>
#include <stdint.h>
#include <mthread.h>
#include <assert.h>
#include <sys/syscall.h>
#include <sys/binsem.h>
#include <os/list.h>

int mthread_spin_init(mthread_spinlock_t *lock)
{
    // TODO:
    return 0;
}
int mthread_spin_destroy(mthread_spinlock_t *lock) {
    // TODO:
    return 0;
}
int mthread_spin_trylock(mthread_spinlock_t *lock)
{
    // TODO:
    return 0;
}
int mthread_spin_lock(mthread_spinlock_t *lock)
{
    // TODO:
    return 0;
}
int mthread_spin_unlock(mthread_spinlock_t *lock)
{
    // TODO:
    return 0;
}

int mthread_mutex_init(mthread_mutex_t *lock)
{
    lock->lock_id = binsemget((int)lock);
    return 1;
}
int mthread_mutex_destroy(mthread_mutex_t *lock) {
    return binsem_destroy(lock->lock_id);
}
int mthread_mutex_trylock(mthread_mutex_t *lock) {
    // TODO:
    return 0;
}
int mthread_mutex_lock(mthread_mutex_t *lock) {
    return binsemop(lock->lock_id, BINSEM_OP_LOCK);
}
int mthread_mutex_unlock(mthread_mutex_t *lock)
{
    return binsemop(lock->lock_id, BINSEM_OP_UNLOCK);
}

int mthread_barrier_init(mthread_barrier_t * barrier, unsigned count)
{
    barrier->total_num = count;
    barrier->wait_num  = 0;
    init_list_head(&barrier->barrier_queue);
}
int mthread_barrier_wait(mthread_barrier_t *barrier)
{
    return sys_barrier_wait(barrier);
}
int mthread_barrier_destroy(mthread_barrier_t *barrier)
{
    if(list_empty(&barrier->barrier_queue)){
        return 1;
    }else{
        return 0;
    }
}

int mthread_cond_init(mthread_cond_t *cond)
{
    init_list_head(&cond->wait_queue);
}
int mthread_cond_destroy(mthread_cond_t *cond) {
    if(list_empty(&cond->wait_queue)){
        return 1;
    }else{
        return 0;
    }
}
int mthread_cond_wait(mthread_cond_t *cond, mthread_mutex_t *mutex)
{
    return sys_cond_wait(cond,mutex);
}
int mthread_cond_signal(mthread_cond_t *cond)
{
    return sys_cond_signal(cond);
}
int mthread_cond_broadcast(mthread_cond_t *cond)
{
    return sys_cond_broadcast(cond);
}

int mthread_semaphore_init(mthread_semaphore_t *sem, int val)
{
    // TODO:
    return 0;
}
int mthread_semaphore_up(mthread_semaphore_t *sem)
{
    // TODO:
    return 0;
}
int mthread_semaphore_down(mthread_semaphore_t *sem)
{
    // TODO:
    return 0;
}
int mthread_semaphore_destroy(mthread_semaphore_t *sem)
{
    // TODO:
    return 0;
}