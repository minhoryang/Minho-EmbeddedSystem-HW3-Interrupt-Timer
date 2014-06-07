#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/unistd.h>
#include <sys/syscall.h>

#define SYSCALL_NUM 366
#define MODULE_DEV "/dev/dev_driver"

int main(int argc, char* argv[]){
	int call, fd;
	if(argc!=4)
		goto error;
	if((call = syscall(SYSCALL_NUM, atoi(argv[1]), atoi(argv[2]), atoi(argv[3]))) == -1)
		goto error;
	printf("Syscall Out: %X\n", call);

	fd = open(MODULE_DEV, O_WRONLY);
	if(fd<0){
		printf("Driver Open Failed\n");
		return 2;
	}
	write(fd, call, sizeof(call));
	close(fd);
	return 0;
error:
	printf("%s interval[1-100] count[1-100] startoption[0001-8000]\n", argv[0]);
	return 1;
}
