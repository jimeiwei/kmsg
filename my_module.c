#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/random.h>

#define TIMER_INTERVAL (1 * HZ)  // 定时器每秒触发一次
#define RUN_TIME (300 * HZ)        // 驱动运行 1 分钟

static struct timer_list my_timer;
static unsigned long start_time;

// 函数前置声明
static void my_module_exit(void);

void timer_callback(struct timer_list *timer) {
    // 生成随机数并打印
    int random_number;
    get_random_bytes(&random_number, sizeof(random_number));
    printk("In the world of technology and innovation, advancements occur at a rapid pace, reshaping how people interact, work, and live. Each day brings new developments in artificial intelligence, data processing, and sustainable solutions, driving economies.\n");

    // 检查是否达到运行时间
    if (time_after(jiffies, start_time + RUN_TIME)) {
        printk(KERN_INFO "Exiting the module after 1 minute.\n");
        my_module_exit();  // 改为直接调用 exit 函数
    } else {
        mod_timer(&my_timer, jiffies + TIMER_INTERVAL);  // 重新设置定时器
    }
}

static int __init my_module_init(void) {
    printk(KERN_INFO "Module initialized. Starting timer...\n");

    start_time = jiffies;  // 记录开始时间
    timer_setup(&my_timer, timer_callback, 0);  // 初始化定时器
    mod_timer(&my_timer, jiffies + TIMER_INTERVAL);  // 设置定时器

    return 0;  // 返回 0 表示模块成功加载
}

static void my_module_exit(void) {
    del_timer(&my_timer);  // 删除定时器
    printk(KERN_INFO "Module exited.\n");
}

module_init(my_module_init);  // 加载模块时调用的函数
module_exit(my_module_exit);   // 卸载模块时调用的函数

MODULE_LICENSE("GPL");          // 模块许可证
MODULE_AUTHOR("Your Name");     // 模块作者
MODULE_DESCRIPTION("A simple timer module that prints random numbers every second.");
