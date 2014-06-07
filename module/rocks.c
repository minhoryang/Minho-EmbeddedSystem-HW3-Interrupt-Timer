struct StopWatch{
	bool isRun;
	bool isPause;
	int min;
	int sec;
};
struct StopWatch my_stopwatch;

void NextTurn(struct StopWatch *sw){
	if(++sw->sec == 60){
		sw->sec = 0;
		sw->min++;
	}
	if(sw->min == 60)
		sw->min = 0;
}

void CurrentTurn(struct StopWatch *sw){
	if(sw->isRun){
		if(!sw->isPause)
			NextTurn(sw);
	}
}

void Reset(struct StopWatch *sw){
	sw->isRun = false;
	sw->isPause = false;
	sw->sec = 0;
	sw->min = 0;
}

irqreturn_t StartButton(int irq, void *dev_id, struct pt_regs *reg){
	printk("STARTBUTTON\n");
	my_stopwatch.isRun = true;
	return IRQ_HANDLED;
}
irqreturn_t PauseButton(int irq, void *dev_id, struct pt_regs *reg){
	printk("PAUSEBUTTON\n");
	if(my_stopwatch.isRun){
		if(my_stopwatch.isPause){
			my_stopwatch.isPause = false;
		}else{
			my_stopwatch.isPause = true;
		}
	}
	return IRQ_HANDLED;
}
irqreturn_t ResetButton(int irq, void *dev_id, struct pt_regs *reg){
	printk("RESETBUTTON\n");
	Reset(&my_stopwatch);
	return IRQ_HANDLED;
}
unsigned int now =0;
irqreturn_t ExitButton(int irq, void *dev_id, struct pt_regs *reg){
	if(now){
		now = 0;
		printk("reset\n");
	}else{
		now = get_jiffies_64();
		printk("set\n");
	}	
	return IRQ_HANDLED;
}
