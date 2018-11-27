#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

unsigned int objnum = 2000;
module_param(objnum, uint, 0);

int threadfn(unsigned int *data){
    printk(KERN_INFO "number of objects: %d\n", data);
    return 0;
}

/* init function - logs that initialization happened, returns success */
static int simple_init (void) {
    struct task_struct* thread;
    printk(KERN_INFO "module initialized\n");
    thread = kthread_create(threadfn, &objnum, "new thread");
    kthread_bind(thread, 0);
    wake_up_process(thread);
    return 0;
}

/* exit function - logs that the module is being removed */
static void simple_exit (void) {
    printk(KERN_INFO "module unloaded\n");
}

module_init (simple_init);
module_exit (simple_exit);

