/*
 * main.c
 * 
Copyright <2020> <Pierre JEANNE>
MIT liscence

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/


#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <poll.h>
#include <string.h>

#include "serial.h"


int port;



int main(int argc, char *argv[])
{
	char *uart_dev = "/dev/ttyUSB0" ;
	int uart_baudrate =115200 ;
	char *message = "hello world \n" ;

	extern char *optarg;
	extern int optind;
	int c;

	
	
	
	printf("my process ID is : %d \n\n \n",getpid());
	
	

	while ( (c=getopt(argc,argv,"u:b:m:")) != EOF )
		switch (c) {
		case 'u':
			uart_dev = optarg;
			break;
		case 'b':
			uart_baudrate = atoi(optarg);
			break;
		case 'm':
			message = optarg;
			break;
		
		case '?':
			
			exit(1);
		}

	printf("uart = %s \n",uart_dev);
	printf("uart baudrate = %d \n",uart_baudrate);
	printf("message = %s \n",message);
	
	struct pollfd fds[1];
	int timeout_msecs = 5000;
	int ret;

    ssize_t length;
    char buff[512]; // serial buffer

	
	port = open_port(uart_dev); 
	if(port != -1)
	{
		set_baudrate(port,uart_baudrate); //det baudrate
		
		fds[0].fd = port;
		fds[0].events = POLLIN ; // we are interested by POLLIN events type
		
		while(1)
		{
			//create echo uart with select
			ret = poll(fds,1, timeout_msecs);
			
			if (ret > 0) {
				
					if (fds[0].revents & POLLIN) // revents are auto cleared after read
					{
						length = read(port, buff, sizeof(buff) );
						buff[length] = '\0';
						for(int i =0;i<length;i++)
						{
							printf("0x%02x ", buff[i]); 
						
						}
						printf("%s \n",buff);
						
						
						
					}
					
					
			}
			else
			{

				write(port,message,strlen(message));
			}
		}
			
		
		
	}
	else
	{
		 perror("error opening serial port exiting");
	}	 exit(1);
	
	
	
	return 0;
}

