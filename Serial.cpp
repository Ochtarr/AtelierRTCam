/**
 * \file Serial.cpp
 * \brief Classe Serial
 * \author Damien.L
 * \version 0.1
 * \date 10 octobre 2017
 *
 * Classe permettant d'initialiser et d'envoyer des données par liaison série USB.
 *
 */

#include "Serial.hpp"

/**
 * \fn int Serial::Init()
 * \brief Méthode permettant d'initialiser la liaison série
 *
 * \return Retourne 0 si erreur, 1 sinon
 */

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
//		return 1;
	}

	if(tcgetattr(fd1, &toptions) == -1)
	{
		printf("Error getting attribut\n");
		return -1;
	}
	
	/* 9600 baud */
	cfsetispeed(&toptions, B115200) ;
	cfsetospeed(&toptions, B115200) ;
	
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

/**
 * \fn void Serial::Close()
 * \brief Méthode permettant de fermer la liaison série
 *
 */

void Serial::Close()
{
	close(fd1);
}

/**
 * \fn void Serial::Write(char *buff)
 * \brief Méthode permettant d'envoyer 8 bits (1 octet) à la liaison série
 *
 */

void Serial::Write(char *buff)
{
	wr=write(fd1,buff,1);
	std::this_thread::sleep_for(std::chrono::milliseconds(20));

}
