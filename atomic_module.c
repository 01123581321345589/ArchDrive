#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/types.h>
#include <asm-generic/atomic.h>

atomic_t race;
atomic_set(&race, 0);

#define iters 1000000
int threadfn(void *data){
    int i, cid = smp_processor_id();
    printk(KERN_DEBUG "loop starts on cpu %d\n", cid);
    for(i=0;i<iters;i++){
        atomic_add(1, &race);
    }
    printk(KERN_DEBUG "loop ends on cpu %d\n", cid);
    return 0;
}

/* init function - logs that initialization happened, returns success */
static int simple_init (void) {
    printk(KERN_INFO "module initialized\n");
    unsigned int k;
    struct task_struct* thread;
    for(k=0;k<4;k++){
        thread = kthread_create(threadfn, NULL, "thread %d", k);
        kthread_bind(thread, k);
        wake_up_process(thread);
    }
    return 0;
}

/* exit function - logs that the module is being removed */
static void simple_exit (void) {
    printk(KERN_NOTICE "race:%d\n", atomic_read(&race));
    printk(KERN_INFO "module unloaded\n");
}

module_init (simple_init);
module_exit (simple_exit);

