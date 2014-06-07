#include <linux/timer.h>

static struct timer_list my_timer;

#define TimerRefresh do{ \
	my_timer.expires = get_jiffies_64() + HZ; \
	add_timer(&my_timer); \
}while(false);

void timer_handler(unsigned int ptr){
	struct StopWatch *p = (struct StopWatch *)ptr;
	CurrentTurn(p);
	TimerRefresh;
}

void timer_init(unsigned int ptr){
	init_timer(&my_timer);
	my_timer.data = ptr;
	my_timer.function = timer_handler;
	TimerRefresh;
}

void timer_exit(){
	del_timer(&my_timer);
}
