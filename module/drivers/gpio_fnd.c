#include "./gpio_fnd.h"

#define FND_GPL2CON 0x11000100  // Pin Configuration
#define FND_GPL2DAT 0x11000104  // Pin Data
#define FND_GPE3CON 0x11400140  // Pin Configuration
#define FND_GPE3DAT 0x11400144  // Pin DAta

//Global variable

static unsigned char *fnd_data;
static unsigned int *fnd_ctrl;
static unsigned char *fnd_data2;
static unsigned int *fnd_ctrl2;

ssize_t gpio_fnd_write(unsigned short fnd_buff)
{
    char fnd_sel;
    char fnd_dat;
    fnd_sel=(char)(fnd_buff>>8);
    fnd_dat=(char)(fnd_buff&0x00FF);

    printk("FND Secect : %d\n",fnd_sel);
    printk("Fnd Data : %d\n",fnd_dat);
	

    outb (fnd_sel,(unsigned int)fnd_data2);
    outb (fnd_dat, (unsigned int)fnd_data);
    return sizeof(fnd_buff);
}

int __init gpio_fnd_init(void)
{
	printk("%s\n", __FUNCTION__);
    fnd_data = ioremap(FND_GPL2DAT, 0x01);
    fnd_data2 = ioremap(FND_GPE3DAT, 0x01);
    if(fnd_data==NULL)
    {
        printk("ioremap failed!\n");
        return -1;
    }

    fnd_ctrl = ioremap(FND_GPL2CON, 0x04);
    fnd_ctrl2 = ioremap(FND_GPE3CON, 0x04);
    if(fnd_ctrl==NULL)
    {
        printk("ioremap failed!\n");
        return -1;
    } else {
        outl(0x11111111,(unsigned int)fnd_ctrl);
	    outl(0x10010110,(unsigned int)fnd_ctrl2);
    }
    outb(0xFF, (unsigned int)fnd_data);
    outb(0xFF, (unsigned int)fnd_data);
    return 0;
}

void __exit gpio_fnd_exit(void)
{
	printk("%s\n", __FUNCTION__);
    outb(0xFF, (unsigned int)fnd_data);
    iounmap(fnd_data);
    iounmap(fnd_data2);
    iounmap(fnd_ctrl);
    iounmap(fnd_ctrl2);
}

