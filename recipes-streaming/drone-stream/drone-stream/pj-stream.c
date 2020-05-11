#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
// " ! ","rtpjpegpay"," ! ","udpsink","host=","192.168.1.78 ","port=","5000",
int
main(int argc, char *argv[])
{
daemon(0, 0);
extern char *optarg;
extern int optind;
char *destination_address = "127.0.0.1" ;
char *destination_port = "5000";
int c=0;
while ( (c=getopt(argc,argv,"d:p:")) != EOF )
		switch (c) {
		
		case 'd':
			destination_address = optarg;
			break;
		case 'p':
			destination_port = optarg;
			break;
		
		case '?':
			
			exit(1);
		}

char *newargv[] = {"gst-launch-1.0","v4l2src","device=/dev/video0","!","video/x-raw,width=640,height=480","!","jpegenc","!","rtpjpegpay","!","udpsink","host=",destination_address,"port=",destination_port,NULL};


    execve("/usr/bin/gst-launch-1.0", newargv,NULL);
    perror("execve");   /* execve() ne retourne qu'en cas d'erreur */
    exit(EXIT_FAILURE);
}
