#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/kthread.h>

volatile int race = 0;
#define iters 1000000
int threadfn(void *data){
    int i;
    printk(KERN_ALERT "cpu:%d\n", smp_processor_id());
    for(i=0;i<iters;i++){
        race++;
    }
    //do_exit();
    return 0;
}
/* init function - logs that initialization happened, returns success */
static int simple_init (void) {
    printk(KERN_ALERT "module initialized\n");
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
    printk(KERN_ALERT "module unloaded\n");
}

module_init (simple_init);
module_exit (simple_exit);

