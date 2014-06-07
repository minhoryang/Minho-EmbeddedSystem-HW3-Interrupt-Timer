#include <linux/timer.h>

static struct timer_list my_timer;

void timer_handler(unsigned int _){
	//current_turn();
}

void timer_init(unsigned int _){
	init_timer(&my_timer);
	my_timer.expires = get_jiffies_64() + HZ * 1;
	my_timer.data = 0;
	my_timer.function = timer_handler;
	add_timer(&my_timer);
}

void timer_add(int wanted){
	my_timer.expires = get_jiffies_64() + wanted * HZ / 10;
	printk("%d\n", HZ);
	add_timer(&my_timer);
}

void timer_exit(){
	del_timer(&my_timer);
}
