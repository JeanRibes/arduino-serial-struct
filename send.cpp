#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
typedef struct __attribute__ ((packed)) cmd_pkt {
//typedef struct cmd_pkt {
  char c;//command
  uint16_t a;//address EEPROM ou port
  uint8_t v;//valeur ou high/low
} cmd_pkt;
unsigned char start=0x2;
unsigned char stop=0x3;
struct termios tio;
struct termios stdio;
struct termios old_stdio;
int tty_fd;

int setup_term(void)
{
        tcgetattr(STDOUT_FILENO,&old_stdio);

//        printf("Please start with %s /dev/ttyS1 (for example)\n",argv[0]);
        memset(&stdio,0,sizeof(stdio));
        stdio.c_iflag=0;
        stdio.c_oflag=0;
        stdio.c_cflag=0;
        stdio.c_lflag=0;
        stdio.c_cc[VMIN]=1;
        stdio.c_cc[VTIME]=0;
        tcsetattr(STDOUT_FILENO,TCSANOW,&stdio);
        tcsetattr(STDOUT_FILENO,TCSAFLUSH,&stdio);
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);       // make the reads non-blocking

        memset(&tio,0,sizeof(tio));
        tio.c_iflag=0;
        tio.c_oflag=0;
        tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
        tio.c_lflag=0;
        tio.c_cc[VMIN]=1;
        tio.c_cc[VTIME]=5;

        tty_fd=open("/dev/ttyACM0", O_RDWR | O_NONBLOCK);
        cfsetospeed(&tio,B9600);            // 115200 baud
        cfsetispeed(&tio,B9600);            // 115200 baud

        tcsetattr(tty_fd,TCSANOW,&tio);
	//write(tty_fd,&c,1);
	return 0;
}
int close_term(void){
        close(tty_fd);
        tcsetattr(STDOUT_FILENO,TCSANOW,&old_stdio);

        return EXIT_SUCCESS;
}
int main(int argc, char** argv){
    if(argc != 4){
	printf("Usage: ./a.out [r|v|l] address value");
    }
    setup_term();
    cmd_pkt msg;
    msg.c = argv[1][0];
    msg.a = atoi(argv[2]);
    msg.v = atoi(argv[3]);
    printf("%c",start);//start
//    printf((char*) &msg, sizeof(cmd_pkt));
    printf((char*) &msg);
    printf("%c",stop);//stop
    write(tty_fd, &start, 1);
    write(tty_fd, &msg, sizeof(cmd_pkt));
    write(tty_fd, &stop, 1);
    close_term();
}


