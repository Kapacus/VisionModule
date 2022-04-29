/* Communication.h
 *
 *  Created on: Apr 9, 2014
 *      Author: steadystill
 */

#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_
#include <cstdio>
#include <iostream>
#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions

class Communication
{
public:
	void openport();
	void closeport();
	int writeUSB(std::string cmd);
	std::string readUSB();
};

#endif /* COMMUNICATION_H_ */
