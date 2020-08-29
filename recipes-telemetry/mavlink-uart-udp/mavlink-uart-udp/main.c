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
#include <sys/types.h> /*  linux sys types */
#include <sys/time.h> /* linux sys time struct*/
#include <sys/select.h> /* select fd */
#include <string.h>
#include <poll.h>
#include <sys/ioctl.h>

#include "serial.h"
#include "udp.h"

int port;



int main(int argc, char *argv[])
{
	daemon(0,0);
	char *uart_dev = "/dev/ttyUSB0" ;
	int uart_baudrate =115200 ;
	char *destination_address = "127.0.0.1" ;
	int destination_port =5000 ;

	extern char *optarg;
	extern int optind;
	int c;

	int port;
	ssize_t ser_length; // serial buffer lenght
    	char ser_buff[512]; // serial buffer


	ssize_t sock_length; // len_socket_buffer
    	char sock_buff[512]; // socket buffer
	extern struct sockaddr_in servaddr; // server struct configured by "open_connection"
	int fromlen = sizeof(servaddr); //used with recvfrom
	int socket;

	
	
	
	printf("my process ID is : %d \n\n \n",getpid());
	
	

	while ( (c=getopt(argc,argv,"u:b:d:p:")) != EOF )
		switch (c) {
		case 'u':
			uart_dev = optarg;
			break;
		case 'b':
			uart_baudrate = atoi(optarg);
			break;
		case 'd':
			destination_address = optarg;
			break;
		case 'p':
			destination_port = atoi(optarg);
			break;		
		case '?':
			
			exit(1);
		}

	printf("uart = %s \n",uart_dev);
	printf("uart baudrate = %d \n",uart_baudrate);
	printf("destination_address = %s \n",destination_address);
	printf("destination_port = %d \n",destination_port);

	struct pollfd fds[2]; // create 2 file descriptor set
	int timeout_msecs = 5000;
	int ret;


	port = open_port(uart_dev); // open serial port
	if(port != -1) // if we succesfully open serial port 
	{
		set_baudrate(port,uart_baudrate); // set uart baudarte
		socket = open_connection(destination_address,destination_port); // open socket at specified ip and port 
		if(socket != -1)
		{
			fds[0].fd = port; // configure fdset 0 on uart 
			fds[0].events = POLLIN ; // we are interested by POLLIN events type

			fds[1].fd = socket; // configure fdset 1 on udp socket
			fds[1].events = POLLIN ; // we are interested by POLLIN events type


			while(1) // main process loop
			{
				ret = poll(fds,2, timeout_msecs); // check the two fds for events (uart & socket)
				if (ret > 0) { // if there is an event

					if (fds[1].revents & POLLIN) // if the event is an input on socket fds[1]
					{
						sock_length = recvfrom(socket,&sock_buff,sizeof(sock_buff),MSG_WAITALL, (struct sockaddr *) &servaddr, &fromlen); //receive from socket and store tosock_buff

						write(port,sock_buff,sock_length); // send  back sock_buff content through uart

						

						
						
					}
					

				
					if (fds[0].revents & POLLIN) // if the event is an input on uart fds[0]
					{
						int available_bytes = 0;

						ioctl(port, FIONREAD, &available_bytes);

						if(available_bytes >=140) // wait get 140 bytes before send
						{


							ser_length = read(port, ser_buff, sizeof(ser_buff) ); //read the uart and store to serbuff

							sendto(socket,ser_buff,ser_length,MSG_CONFIRM, (const struct sockaddr *) &servaddr,sizeof(servaddr)); //send the serial buffer via udp socket

						}
						else
						{
							fds[0].revents = 0;
							usleep(10000);
							//printf("%d bytes available on uart \n ",available_bytes);
							
						}

					
						
					}
					
					
				}
				else  // if nothing append before "timeout_msecs" milliseconds (5000)
				{
					printf("timeout \n");
					
				}
			}

		}
		else
		{
			perror("error opening udp socket");
		 	exit(1);
		}	
		
		
	}
	else
	{
		 perror("error opening serial port exiting");
		 exit(1);
	}
	
	
	
	return 0;
}

