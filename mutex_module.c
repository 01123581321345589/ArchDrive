#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/mutex.h>

#define iters 1000000

volatile int race = 0;
DEFINE_MUTEX(m1);

int threadfn(void *data){
    int i;
    int unsigned cid = smp_processor_id();
    printk(KERN_DEBUG "loop starts on cpu %d\n", cid);
    for(i=0;i<iters;i++){
        mutex_lock(&m1);
        race++;
        mutex_unlock(&m1);
    }
    printk(KERN_DEBUG "loop ends on cpu %d\n", cid);
    return 0;
}

/* init function - logs that initialization happened, returns success */
static int simple_init (void) {
    int unsigned k;
    struct task_struct* thread;
    printk(KERN_INFO "module initialized\n");
    for(k=0;k<4;k++){
        thread = kthread_create(threadfn, NULL, "thread %d", k);
        kthread_bind(thread, k);
        wake_up_process(thread);
    }
    return 0;
}

/* exit function - logs that the module is being removed */
static void simple_exit (void) {
    printk(KERN_NOTICE "race:%d\n", race);
    printk(KERN_INFO "module unloaded\n");
}

module_init (simple_init);
module_exit (simple_exit);

