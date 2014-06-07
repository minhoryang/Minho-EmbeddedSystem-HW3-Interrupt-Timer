#include <linux/init.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <asm/ioctl.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <mach/gpio.h>
#include <mach/regs-gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/ioport.h>
#include <linux/slab.h>
#include <linux/wait.h>

int interruptCount = 0;
wait_queue_head_t wq_write;
DECLARE_WAIT_QUEUE_HEAD(wq_write);

#include "./drivers/gpio_fnd.c"
#include "./rocks.c"
void timer_add(int wanted);
#include "./timer.c"

#define DEV_MAJOR 245
#define DEV_NAME "stopwatch"


static int usage = 0;


// XXX : No Usage Counter.
int _open(struct inode *minode, struct file *mfile){
	if(usage)
		return -EBUSY;
	usage = 1;
	interruptCount = 0;
	printk("20091631 Open\t\n");
	return 0;
}
int _release(struct inode *minode, struct file *mfile){
	usage = 0;
	interruptCount = 0;
	printk("20091631 Release\t\n");
	return 0;
}
ssize_t _write(struct file *mfile, const char *gdata, size_t length, loff_t *off_what){
	int ret;
	if(interruptCount == 0){
		ret = request_irq(gpio_to_irq(S5PV310_GPX2(0)), &StartButton, IRQF_TRIGGER_RISING, "X2.0", NULL);
		ret = request_irq(gpio_to_irq(S5PV310_GPX2(1)), &PauseButton, IRQF_TRIGGER_RISING, "X2.1", NULL);
		ret = request_irq(gpio_to_irq(S5PV310_GPX2(2)), &ResetButton, IRQF_TRIGGER_RISING, "X2.2", NULL);
		ret = request_irq(gpio_to_irq(S5PV310_GPX2(4)), &ExitButton, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "X2.4", NULL);
		Reset(&my_stopwatch);
		timer_init(&my_stopwatch);
		printk("SleepStart!\n");
		//interruptible_sleep_on(&wq_write);
		now = 0;
		while(true){
			gpio_fnd_write(my_gpio_fnd(1, my_stopwatch.min / 10));
			msleep(5);
			gpio_fnd_write(my_gpio_fnd(2, my_stopwatch.min % 10));
			msleep(5);
			gpio_fnd_write(my_gpio_fnd(3, my_stopwatch.sec / 10));
			msleep(5);
			gpio_fnd_write(my_gpio_fnd(4, my_stopwatch.sec % 10));
			msleep(5);
			if(now){
				if(get_jiffies_64() - now >= 3 * HZ){
					break;
				}
			}
		}
		gpio_fnd_write(0xFF);
		timer_exit();
		free_irq(gpio_to_irq(S5PV310_GPX2(0)), NULL);
		free_irq(gpio_to_irq(S5PV310_GPX2(1)), NULL);
		free_irq(gpio_to_irq(S5PV310_GPX2(2)), NULL);
		free_irq(gpio_to_irq(S5PV310_GPX2(4)), NULL);
	}
	return 0;
}
static struct file_operations _fops = {
	.open = _open,
	.release = _release,
	.write = _write,
};


int __init dev_device_init(void){
	int result;
	if((result = register_chrdev(DEV_MAJOR, DEV_NAME, &_fops)) < 0){
		printk(KERN_WARNING "Failed to register.\n");
		return result;
	}
	if(gpio_fnd_init())
		goto failed;

	printk("20091631 Init\n");
	interruptCount = 0;

	return 0;
failed:
	gpio_fnd_exit();
	return 1;
}
void __exit dev_device_exit(void){
	timer_exit();
	gpio_fnd_exit();
	unregister_chrdev(DEV_MAJOR, DEV_NAME);
	printk("20091631 Exit\n");
}
module_init(dev_device_init);
module_exit(dev_device_exit);
MODULE_AUTHOR("20091631MINHORYANG");
MODULE_LICENSE("GPL");
