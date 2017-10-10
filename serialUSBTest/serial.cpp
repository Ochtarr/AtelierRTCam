#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;

int fd1;
int wr,rd;
char *buffReceive;

int main()
{
	char *buff = new char[1];

	fd1=open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);

	if (fd1 == -1 )
	{
		perror("open_port: Unable to open /dev/ttyACM0");
	}

	else
	{
		fcntl(fd1, F_SETFL,0);
		printf("Porc ouvert, on continue.\n");

		//write
		sprintf(buff, "%c", 0b10110001);
		wr=write(fd1,buff,1);

		close(fd1);
	}

return 0;
}
