
#include <cstdio>
#include <iostream>
#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions
#include "Communication.h"

using namespace std;
int USB;
//char dev[] = "/dev/ttyUSB3";
char dev[] = "/dev/ttyO2";

void Communication::openport(){
	/* Open port */
	USB = open( dev, O_RDWR| O_NOCTTY ); //O_RDWR Opens for read and write

	/* Error Handling */
	if ( USB < 0 )
	{
		cout << "Error " << errno << " opening " << dev << ": " << strerror (errno) << endl;
	}
	/* Configure port */
	struct termios tty;
	struct termios tty_old;
	memset (&tty, 0, sizeof tty);

	/* Error Handling */
	if ( tcgetattr ( USB, &tty ) != 0 )
	{
		cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << endl;
	}

	/* Save old tty parameters */
	tty_old = tty;

	/* Set Baud Rate */
	cfsetospeed (&tty, (speed_t)B9600);
	cfsetispeed (&tty, (speed_t)B9600);

	/* Setting other Port Stuff */
	tty.c_cflag     &=  ~PARENB;        // Make 8n1
	tty.c_cflag     &=  ~CSTOPB;
	tty.c_cflag     &=  ~CSIZE;
	tty.c_cflag     |=  CS8;

	tty.c_cflag     &=  ~CRTSCTS;       // no flow control
	tty.c_cc[VMIN]      =   1;                  // read doesn't block
	tty.c_cc[VTIME]     =   5;                  // 0.5 seconds read timeout
	tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

	/* Make raw */
	cfmakeraw(&tty);

	/* Flush Port, then applies attributes */
	tcflush( USB, TCIFLUSH );
	if ( tcsetattr ( USB, TCSANOW, &tty ) != 0)
	{
		cout << "Error " << errno << " from tcsetattr" << endl;
	}

}

void Communication::closeport(){
	close(USB);
}

int Communication::writeUSB(string cmd_string){
	cmd_string = "\x0002" + cmd_string+ "\x0003"; //Append START and END 
	/* *** WRITE *** */
	int n_written = 0;
	char cmd[cmd_string.length()];
	strcpy(cmd, cmd_string.c_str());
	
	do {
		n_written += write( USB, &cmd[n_written], 1 ); //Send command cmd
	}
	while (n_written < strlen(cmd));
	return 0;
}

string Communication::readUSB(){
	/* Allocate memory for read buffer */
	char buf [256];
	memset (&buf, '\0', sizeof buf);

	/* *** READ *** */
	int n = 0; //Changed for test
	int count = 0;
	string response;
	bool flag_end =false;
	bool flag_start =false;	
	//cout << "Read init" << endl;
	while(!flag_end)
	{
		n = read(USB, &buf, 1);
		count++;
		//cout << buf << endl;
		if(flag_start){
			if(strcmp(buf,"\x0003")){ //Check for END
				response.append(buf);  //Changed from &buf (error)"/r"
			}else{
				flag_end = true;
			}
		}
		if(!strcmp(buf,"\x0002")){ //Check for START
			flag_start = true;
		}
	}
	
	if (n < 0)
	{
		cout << "Error reading: " << strerror(errno) << endl;
	}
	else if (n == 0)
	{
		cout << "Read nothing!" << endl;
	}
	return response;
}
