/*
 * Serial.cpp
 *
 *  Created on: 10 oct. 2017
 *      Author: damien
 */

#include "Serial.hpp"

int Serial::Init()
{
	fd1=open("/dev/ttyACM0", O_RDWR | O_NONBLOCK);

	if (fd1 == -1 )
	{
		return 0;
	}
	else
	{
		fcntl(fd1, F_SETFL,0);
		return 1;
	}

	if(tcgetattr(fd1, &toptions) == -1)
	{
		printf("Error getting attribut\n");
		return -1;
	}
	
	/* 9600 baud */
	cfsetispeed(&toptions, B9600) ;
	cfsetospeed(&toptions, B9600) ;
	
	/* 8 bits, no parity, no stop bits */
	toptions.c_cflag &= ~PARENB ;
	toptions.c_cflag &= ~CSTOPB ;
	toptions.c_cflag &= ~CSIZE;
	toptions.c_cflag |= CS8 ;
	
	/* no hardware flow control */
	toptions.c_cflag &= ~CRTSCTS;
	
	toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
	toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl
	
	toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
	toptions.c_oflag &= ~OPOST; // make raw
	
	
	toptions.c_cc[VMIN]  = 0;
	toptions.c_cc[VTIME] = 0;
	
	if(tcsetattr(fd1, TCSANOW, &toptions) == -1){
		printf("Error setting attribut\n");
		return -1;
	}
	
	usleep(1000*1000);
	
	tcflush(fd1, TCIFLUSH);

	printf("Init OK\n");
}

void Serial::Close()
{
	close(fd1);
}

void Serial::Write(char *buff)
{
	wr=write(fd1,buff,1);
}