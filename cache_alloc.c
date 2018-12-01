#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/jiffies.h>
#include <linux/kthread.h>
#include <linux/moduleparam.h>
#include <linux/gfp.h>
#include <linux/log2.h>
#include <linux/time.h>
#include <linux/slab.h>

unsigned int objnum = 2000;
module_param(objnum, uint, 0);

struct kmem_cache *cache;
struct type {
    unsigned int arr[8];
};

int threadfn(void *data){
    struct timeval t0, t1;
    void *p;
    int i;
    printk(KERN_INFO "allocating cache for %u objects\n", objnum);
    do_gettimeofday(&t0);
    for(i=0;i<objnum;i++){
        p = kmem_cache_alloc(cache, GFP_KERNEL);
        kmem_cache_free(cache, p);
    }
    do_gettimeofday(&t1);
    printk(KERN_INFO "takes %ld microseconds\n", t1.tv_usec-t0.tv_usec);
    return 0;
}

/* init function - logs that initialization happened, returns success */
static int simple_init (void) {
    struct task_struct* thread;
    printk(KERN_INFO "\e[36mmodule initialized\e[0m\n");
    cache = kmem_cache_create("cache", sizeof(struct type), 0, 0, NULL);
    thread = kthread_create(threadfn, NULL, "new thread");
    kthread_bind(thread, 0);
    wake_up_process(thread);
    return 0;
}

/* exit function - logs that the module is being removed */
static void simple_exit (void) {
    kmem_cache_destroy(cache);
    printk(KERN_INFO "\e[36mmodule unloaded\e[0m\n");
}

module_init (simple_init);
module_exit (simple_exit);

