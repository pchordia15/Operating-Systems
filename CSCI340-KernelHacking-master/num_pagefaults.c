#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros
#include <linux/proc_fs.h>   // included for adding proc file and writing to it
#include <linux/seq_file.h>  // included for using the sequence files
#include <linux/mm.h>        // included for virtual machine statistics also vm mannager

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Priyanka");
MODULE_DESCRIPTION("Linux Kernel module to provide pagefaults");

const char name[] = "num_pagefaults";
static struct proc_dir_entry* proc_file; 

static int procf_show(struct seq_file *m, void *v)
{
    unsigned long stats_buffer[NR_VM_EVENT_ITEMS];
    all_vm_events(stats_buffer);
    seq_printf(m, "%lu\n", stats_buffer[PGFAULT]);
    return 0;
}

static int procf_open(struct inode *inode, struct file *file)
{
    return single_open(file, procf_show, NULL);
} 

static const struct file_operations procf_fops = {
    .owner    = THIS_MODULE,
    .open     = procf_open,
    .read     = seq_read,        
    .llseek   = seq_lseek,       
    .release  = single_release,  
};

static int __init procf_init(void)
{
    proc_file = proc_create(name,0,NULL,&procf_fops);
    return 0;
}

static void __exit procf_exit(void)
{
    remove_proc_entry(name,NULL);
}

module_init(procf_init);
module_exit(procf_exit);
MODULE_LICENSE("GPL");
