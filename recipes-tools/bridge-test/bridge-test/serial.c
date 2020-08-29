
#include "serial.h"

void set_baudrate(int fd,unsigned int speed)
{
	
	struct termios config; // declaration d'un structure termios
	tcgetattr(fd, &config); // get current port state and store in termios struct
	switch(speed)
	{
		case 9600:
			cfsetispeed(&config, B9600); // set baudrate RX to 9600
			cfsetospeed(&config, B9600); // set baudrate TX to 9600
		break;
		case 19200:
			cfsetispeed(&config, B19200); // set baudrate RX to 19200
			cfsetospeed(&config, B19200); // set baudrate TX to 19200
		break;
		case 57600:
			cfsetispeed(&config, B57600); // set baudrate RX to 57600
			cfsetospeed(&config, B57600); // set baudrate TX to 57600
		break;
		case 115200:
			cfsetispeed(&config, B115200); // set baudrate RX to 115200
			cfsetospeed(&config, B115200); // set baudrate TX to 115200
		break;
		default:
			cfsetispeed(&config, B9600); // set baudrate RX to 9600
			cfsetospeed(&config, B9600); // set baudrate TX to 9600
		break;
	}
	
	config.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity   */
	config.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
	config.c_cflag &= ~CSIZE;	 /* Clears the mask for setting the data size             */
	config.c_cflag |=  CS8;      /* Set the data bits = 8                                 */

	config.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
	config.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */ 
	
	
	config.c_iflag &= ~(IXON | IXOFF | IXANY);          /* Disable XON/XOFF flow control both i/p and o/p */
	config.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode                            */

	config.c_oflag &= ~OPOST;/*No Output Processing*/

	tcsetattr(fd, TCSANOW, &config); // write new parameter NOW! to serial port
	
}

int open_port(char * port)
{
  int fd; /* File descriptor */


  fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1)
  {

    char str[50];
    sprintf(str, "open_port: Unable to open %s \n ", port);
    perror(str);
  }
  else
  {
    fcntl(fd, F_SETFL, 0);
  }
  return (fd);
}

