#include <linux/init.h> // contains __init and __exit macros
#include <linux/kernel.h> /* header which gets used for module builds is the header which is part of the kernel source. When modules are built in the kernel source tree, that’s the version which is used.*/
#include <linux/module.h> //Dynamic loading of modules into the kernel.
#include<linux/sched.h> // which contains the scheduling parameters required for implementation of each supported scheduling policy
#include <linux/init_task.h> // INIT_TASK is used to set up the first task table
#include<linux/string.h>

void print_tree(struct task_struct *root) // structure that acts as the process descriptor, holding everything the kernel might need to know about a processes
{
    struct list_head *theList;
    struct task_struct *new;
    struct task_struct *new_copy;
    list_for_each(theList, &root->children)
    {
      char separator[100] = " ";
      new = list_entry(theList, struct task_struct, sibling); // if 'sibbling' , not used then parent processes display , basically to display which all child prcs run
      new_copy = new;
      while(new_copy->parent != &init_task) // circular dbl ll
      {
        new_copy = new_copy->parent; // move ahead to check next parent  prcs , under single main parent
        strcat(separator, " ");
      }  
      printk("%s ->[%d] %s\t%ld\n",separator, new->pid, new->comm,new->state); //unction from the Linux kernel interface that prints messages to the kernel log , to print on console from main memory
      

      if((&new->children)->next != &new->children)  // move ahead to check next sibling  prcs , under single parent
        print_tree(new);
    }
}

/* This function is called when the module is loaded. */
int __init simple_init(void)  //__init macro causes the init function to be discarded and its memory freed once the init function finishes for built-in drivers, but not loadable modules.
{                         // Convert the written text to an integer and store in a global variable . can reduce the amount of memory used by the kernel
printk(KERN_INFO "Loading Module\n");

print_tree(&init_task); // print directories from init task in tree format

printk(KERN_INFO "Module loaded.\n");
return 0;
}


/* This function is called when the module is removed. */
void __exit simple_exit(void) //__exit macro causes the omission of the function when the module is built into the kernel,
{
printk(KERN_INFO "Removed the Module\n");
}
/* Macros for registering module entry and exit points. */
module_init(simple_init); // evry time device driver starts , this  function calls from start
module_exit(simple_exit); //evry time device driver starts , this  function calls from start

MODULE_LICENSE("GPL");
//The license will define how your source code should be shared (or not) with other developers
//systemd joiurnal :collects and stores logging data. It creates and maintains structured, indexed journals based on logging information 
// process state : 
// 0 - running
// 1 - interuptable
// 2 - non interuptable
// 128 - zombie process
// 1026 - uninteruptable and unloadable