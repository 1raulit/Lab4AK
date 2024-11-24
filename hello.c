// SPDX-License-Identifier: Dual BSD/GPL
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/list.h>
#include <linux/slab.h>

MODULE_AUTHOR("Illya Miheev");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static uint repeat = 1;
module_param(repeat, uint, 0444);
MODULE_PARM_DESC(repeat, "The number of times to print 'Hello, world!'");

struct hello_time {
	struct list_head list;
	ktime_t time;
};

static LIST_HEAD(hello_list);

static int __init hello_init(void)
{
	struct hello_time *new_node;
	unsigned int i;

	if (repeat > 10) {
		printk(KERN_ERR "Error: repeat > 10\n");
		return -EINVAL;
	} else if (repeat == 0 || (repeat >= 5 && repeat <= 10)) {
		printk(KERN_WARNING "Warning: repeat is 0 or between 5 and 10\n");
	}

	for (i = 0; i < repeat; i++) {
		new_node = kmalloc(sizeof(*new_node), GFP_KERNEL);
		if (!new_node)
			return -ENOMEM;

		new_node->time = ktime_get();
		list_add_tail(&new_node->list, &hello_list);
		printk(KERN_INFO "Hello, world!\n");
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct hello_time *current_node, *tmp;

	list_for_each_entry_safe(current_node, tmp, &hello_list, list) {
		printk(KERN_INFO "Time: %llu ns\n",
		ktime_to_ns(current_node->time));
		list_del(&current_node->list);
		kfree(current_node);
	}
}

module_init(hello_init);
module_exit(hello_exit);

