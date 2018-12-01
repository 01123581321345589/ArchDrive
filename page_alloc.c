#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/jiffies.h>
#include <linux/kthread.h>
#include <linux/moduleparam.h>
#include <linux/gfp.h>
#include <linux/log2.h>
#include <linux/math64.h>

unsigned int objnum = 2000;
module_param(objnum, uint, 0);

struct type {
    unsigned int arr[8];
};

int threadfn(void *data){
    unsigned long tick0, tick1;
    printk(KERN_INFO "number of objects: %u\n", *(unsigned int *)data);
    printk(KERN_INFO "size of the structure: %zd\n", sizeof(struct type));
    printk(KERN_INFO "%ld/n", order_base_2(objnum)-order_base_2(div_u64(PAGE_SIZE/sizeof(struct type))))
    tick0 = jiffies;
    //alloc_pages(GFP_KERNEL, order_base_2(objnum)-order_base_2(PAGE_SIZE/sizeof(struct type)));
    
    tick1 = jiffies;
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

