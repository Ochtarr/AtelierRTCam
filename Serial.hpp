/*
 * Serial.h
 *
 *  Created on: 10 oct. 2017
 *      Author: damien
 */

#ifndef SERIAL_HPP_
#define SERIAL_HPP_
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <thread>

class Serial {
protected:
	int fd1;
	int wr;
	struct termios toptions;
public:
	int Init();
	void Close();
	void Write(char *buf);
};

#endif /* SERIAL_HPP_ */
