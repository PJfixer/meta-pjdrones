
#include "udp.h"


struct sockaddr_in servaddr;

int open_connection(char * IPaddr,unsigned int port)
{
	int sockfd = 0;
	if (( sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) 
	{ 
		perror("socket creation failed"); 
		return -1;
    } 
    memset(&servaddr, 0, sizeof(servaddr)); 
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(port); 
    servaddr.sin_addr.s_addr = inet_addr(IPaddr); 
    
    return sockfd;
	
}
