#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define MODULE_DEV "/dev/stopwatch"

int main(){
	int fd = open(MODULE_DEV, O_WRONLY);
	if(fd<0){
		printf("Driver Open Failed\n");
		return 2;
	}
	write(fd, 0, 0);
	close(fd);
	return 0;
}
