#include <linux/init.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <asm/ioctl.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <mach/gpio.h>
#include <mach/regs-gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/ioport.h>
#include <linux/slab.h>

#include "./drivers/gpio_fnd.c"
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
	printk("20091631 Open\t\n");
	return 0;
}
int _release(struct inode *minode, struct file *mfile){
	usage = 0;
	printk("20091631 Release\t\n");
	return 0;
}
ssize_t _write(struct file *mfile, const char *gdata, size_t length, loff_t *off_what){
	//int data = (int)gdata;
	//printk("20091631 WRITE\t%X\n", data);
	//revert(data);
	timer_init(0);
	//return sizeof(data);
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

	//draw_clean();
	return 0;
failed:
	gpio_fnd_exit();
	return 1;
}
void __exit dev_device_exit(void){
	timer_exit();
	//draw_clean();
	gpio_fnd_exit();
	unregister_chrdev(DEV_MAJOR, DEV_NAME);
	printk("20091631 Exit\n");
}
module_init(dev_device_init);
module_exit(dev_device_exit);
MODULE_AUTHOR("20091631MINHORYANG");
MODULE_LICENSE("GPL");
