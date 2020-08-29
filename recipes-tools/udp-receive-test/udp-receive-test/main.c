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

#include "udp.h"






int main(int argc, char *argv[])
{
	char *destination_address = "127.0.0.1" ;
	int destination_port =5000 ;
	char *message = "hello world \n" ;

	extern char *optarg;
	extern int optind;
	int c;

	ssize_t length; // len_socket_buffer
    	char buff[512]; // socket buffer
	extern struct sockaddr_in servaddr; // server struct configured by "open_connection"
	int fromlen = sizeof(servaddr);
	int socket;
	
	
	printf("my process ID is : %d \n\n \n",getpid());
	
	

	while ( (c=getopt(argc,argv,"d:p:m:")) != EOF )
		switch (c) {
		case 'd':
			destination_address = optarg;
			break;
		case 'p':
			destination_port = atoi(optarg);
			break;
		case 'm':
			message = optarg;
			break;
		
		case '?':
			
			exit(1);
		}

	printf("destination_address = %s \n",destination_address);
	printf("destination_port = %d \n",destination_port);
	printf("message = %s \n",message);
	
	struct pollfd fds[1];
	int timeout_msecs = 5000;
	int ret;
    	
   

	
	socket = open_connection(destination_address,destination_port); 
	if(socket != -1)
	{
	
		
		fds[0].fd = socket;
		fds[0].events = POLLIN ; // we are interested by POLLIN events type
		
		while(1)
		{
			//create echo udp with poll
			ret = poll(fds,1, timeout_msecs);
			
			if (ret > 0) {
				
					if (fds[0].revents & POLLIN) 
					{
						//length = read(port, buff, sizeof(buff) );
						length = recvfrom(socket,&buff,512,MSG_WAITALL, (struct sockaddr *) &servaddr, &fromlen); 
						buff[length] = '\0';
						printf("%s\n",buff);
						
					}
					
					
			}
			else
			{
				printf("send timeout \n");
				sendto(socket,message,strlen(message),MSG_CONFIRM, (const struct sockaddr *) &servaddr,sizeof(servaddr)); // here we push the data
			}
		}
			
		
		
	}
	else
	{
		 perror("error opening serial port exiting");
	}	 exit(1);
	
	
	
	return 0;
}

