#define gpio_fnd_loc_1 0x02
#define gpio_fnd_loc_2 0x04
#define gpio_fnd_loc_3 0x10
#define gpio_fnd_loc_4 0x80

#define gpio_fnd_num_0 0x02
#define gpio_fnd_num_1 0x9f
#define gpio_fnd_num_2 0x25
#define gpio_fnd_num_3 0x0d
#define gpio_fnd_num_4 0x99
#define gpio_fnd_num_5 0x49
#define gpio_fnd_num_6 0xc1
#define gpio_fnd_num_7 0x1f
#define gpio_fnd_num_8 0x01
#define gpio_fnd_num_9 0x09
#define gpio_fnd_num_X 0xFF


short my_gpio_fnd_loc[5] = {
	0x00,
	gpio_fnd_loc_1,
	gpio_fnd_loc_2,
	gpio_fnd_loc_3,
	gpio_fnd_loc_4
};

short my_gpio_fnd_num[9] = {
	gpio_fnd_num_X,
	0x73,//gpio_fnd_num_1,
	0x8f,//gpio_fnd_num_2,
	0x71,//gpio_fnd_num_3,
	0x8d,//gpio_fnd_num_4,
	0x61,//gpio_fnd_num_5,
	0x0d,//gpio_fnd_num_6,
	0x21,//gpio_fnd_num_7,
	0x05//gpio_fnd_num_8
};

short my_gpio_fnd(int a, int b){
	return (my_gpio_fnd_loc[a] << 8) | my_gpio_fnd_num[b];
}
