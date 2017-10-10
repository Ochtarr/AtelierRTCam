/*
 * Serial.cpp
 *
 *  Created on: 10 oct. 2017
 *      Author: damien
 */

#include "Serial.hpp"

int Serial::Init()
{
	fd1=open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);

	if (fd1 == -1 )
	{
		return 0;
	}
	else
	{
		fcntl(fd1, F_SETFL,0);

		return 1;
	}
}

void Serial::Close()
{
	close(fd1);
}

void Serial::Write(char *buff)
{
	wr=write(fd1,buff,1);
}
