
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/list.h>
#include <linux/slab.h>

MODULE_AUTHOR("Illya Miheev");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

// Параметр для визначення кількості повторів
static uint repeat = 1;
module_param(repeat, uint, 0444);
MODULE_PARM_DESC(repeat, "The number of times to print 'Hello, world!'");

struct hello_time {
    struct list_head list;
    ktime_t time;
};

static LIST_HEAD(hello_list);

// Функція ініціалізації
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

// Функція завершення
static void __exit hello_exit(void)
{
    struct hello_time *current_node, *tmp;

    list_for_each_entry_safe(current_node, tmp, &hello_list, list) {
        printk(KERN_INFO "Time: %llu ns\n", ktime_to_ns(current_node->time));
        list_del(&current_node->list);
        kfree(current_node);
    }
}

module_init(hello_init);
module_exit(hello_exit);

