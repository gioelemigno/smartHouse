#include "UART.h"
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>

#define BUFFER_SIZE 1024

#define OPEN_MODE       O_RDWR | O_NOCTTY | O_SYNC | O_NONBLOCK
#define OPEN_DEVICE     "/dev/ttyUSB0"

#ifdef PRINT_ERROR
    static inline void report_error();
#endif

static inline double getTime_us();

int fd;

uint8_t buffTX[BUFFER_SIZE];
uint8_t buffRX[BUFFER_SIZE];

Buffer_t RXBuffer = {
    .buffer = buffRX,
    .size = 0,
    .max_size = BUFFER_SIZE
};

Buffer_t TXBuffer = {
    .buffer = buffTX,
    .size = 0,
    .max_size = BUFFER_SIZE
};

int UART_init(unsigned long baud_rate){

    fd = open(OPEN_DEVICE, OPEN_MODE);
    if(fd == -1){
        #ifdef PRINT_ERROR
            report_error(__func__);
        #endif
        return -1;
    }

    speed_t myBaud;
    switch(baud_rate){  // code from WiringPi library
        case      50:	myBaud =      B50 ; break ;
        case      75:	myBaud =      B75 ; break ;
        case     110:	myBaud =     B110 ; break ;
        case     134:	myBaud =     B134 ; break ;
        case     150:	myBaud =     B150 ; break ;
        case     200:	myBaud =     B200 ; break ;
        case     300:	myBaud =     B300 ; break ;
        case     600:	myBaud =     B600 ; break ;
        case    1200:	myBaud =    B1200 ; break ;
        case    1800:	myBaud =    B1800 ; break ;
        case    2400:	myBaud =    B2400 ; break ;
        case    4800:	myBaud =    B4800 ; break ;
        case    9600:	myBaud =    B9600 ; break ;
        case   19200:	myBaud =   B19200 ; break ;
        case   38400:	myBaud =   B38400 ; break ;
        case   57600:	myBaud =   B57600 ; break ;
        case  115200:	myBaud =  B115200 ; break ;
        case  230400:	myBaud =  B230400 ; break ;
        case  460800:	myBaud =  B460800 ; break ;
        case  500000:	myBaud =  B500000 ; break ;
        case  576000:	myBaud =  B576000 ; break ;
        case  921600:	myBaud =  B921600 ; break ;
        case 1000000:	myBaud = B1000000 ; break ;
        case 1152000:	myBaud = B1152000 ; break ;
        case 1500000:	myBaud = B1500000 ; break ;
        case 2000000:	myBaud = B2000000 ; break ;
        case 2500000:	myBaud = B2500000 ; break ;
        case 3000000:	myBaud = B3000000 ; break ;
        case 3500000:	myBaud = B3500000 ; break ;
        case 4000000:	myBaud = B4000000 ; break ;

        default:   
            #ifdef PRINT_ERROR
                fprintf(stderr, "%s: Baud rate not supported\n", __func__);
            #endif

            return -1; 
        break;
    }

    //termios setting
    struct termios uart_setting;
    int res;    
    memset(&uart_setting, 0, sizeof(uart_setting));

    res = tcgetattr(fd, &uart_setting);
    if(res == -1){
        #ifdef PRINT_ERROR
            report_error(__func__);
        #endif
        return -1;
    }
    
    res = cfsetispeed(&uart_setting, myBaud);
    if(res == -1){
        #ifdef PRINT_ERROR
            report_error(__func__);
        #endif
        return -1;
    }
    res = cfsetospeed(&uart_setting, myBaud);
    if(res == -1){
        #ifdef PRINT_ERROR
            report_error(__func__);
        #endif
        return -1;
    }
    
    // setting to raw mode
    /*
        cfmakeraw() sets the terminal to something like the "raw" mode of the old Version 7 terminal 
        driver: input is available character by character, echoing is disabled, and all special 
        processing of terminal input and output characters is disabled. 
        The terminal attributes are set as follows:
        
        termios_p->c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
        termios_p->c_oflag &= ~OPOST;
        termios_p->c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
        termios_p->c_cflag &= ~(CSIZE | PARENB);
        termios_p->c_cflag |= CS8;
    */
    cfmakeraw(&uart_setting);
 
    
    //uart_setting.c_cflag &= ~CSTOPB; //set 1 stop bit 
    uart_setting.c_cflag |= CSTOPB; //set 2 stop bit 
    
    //USELESS CODE (same setting of raw mode)
    uart_setting.c_cflag &= ~CSIZE ;
    uart_setting.c_cflag |= CS8 ;
    
    res=tcsetattr(fd, TCSANOW, &uart_setting);
    if(res == -1){
        #ifdef PRINT_ERROR
            report_error(__func__);
        #endif
        return -1;
    }

    usleep(10E03); //wait 10mS
    return 0;
}

int UART_read(Buffer_t* buffer){
    int res = read(fd, buffer->buffer, buffer->max_size);
    if(res == -1){
        #ifdef PRINT_ERROR
            report_error(__func__);
        #endif
        return -1;
    }
    buffer->size = res;
    
    #ifdef DEBUG_UART
        printf("***************************************\n");
        printf("%s Readed %d Bytes\n", __func__, res);
        printBuffer(buffer);
        printf("---------------------------------------\n");
    #endif
    return 0;
}

int UART_read_waiting(Buffer_t* buffer, double* timeout_us){

    #ifdef DEBUG_UART 
        printf("**************** %s ***********************\n", __func__);
    #endif

    uint8_t serial_available = 0;
    int res = ioctl(fd, FIONREAD, &serial_available);
    if(res == -1){
        #ifdef PRINT_ERROR
            report_error(__func__);
        #endif
        return -1;
    }

    if(!serial_available){
        double time_now = getTime_us();
        double time_start = time_now;
        double timeout = *timeout_us;
        
        while(time_now - time_start < timeout && !serial_available){
            time_now = getTime_us();
            res = ioctl(fd, FIONREAD, &serial_available);
            if(res == -1){
                #ifdef PRINT_ERROR
                    report_error(__func__);
                #endif
                return -1;
            }
        }
        #ifdef DEBUG_UART
            printf("%s: Waited %.2fuS\n", __func__, time_now-time_start);
            *timeout_us = *timeout_us - (time_now-time_start);
        #endif

        if(!serial_available){
            #ifdef DEBUG_UART
                printf("---------------------------------------\n");
            #endif

            return -1;
        }
        
    }
    
    res = read(fd, buffer->buffer, buffer->max_size);
    if(res == -1){
        #ifdef PRINT_ERROR
            report_error(__func__);
        #endif
        return -1;
    }
    buffer->size = res;
    
    #ifdef DEBUG_UART
        printf("%s Readed %d Bytes\n", __func__, res);
        printBuffer(buffer);
        printf("---------------------------------------\n");
    #endif
    return 0;
}

int UART_write(Buffer_t* buffer){
    #ifdef DEBUG_UART 
        printf("**************** %s ***********************\n", __func__);
    #endif
    int res = write(fd, buffer->buffer, buffer->size);
    if(res == -1){
        #ifdef PRINT_ERROR
            report_error(__func__);
        #endif
        return -1;
    }

	int resD = tcdrain(fd); // wait for trasmit end
    if(res == -1){
        #ifdef PRINT_ERROR
            report_error(__func__);
        #endif
        return -1;
    }

    #ifdef DEBUG_UART
        printf("%s Written %d Bytes\n", __func__, res);
        printBuffer(buffer);
        printf("----------------------------------------\n");
    #endif
    return 0;
}

int UART_close(){
    int res = close(fd);
    if(res == -1){
        #ifdef PRINT_ERROR
            report_error(__func__);
        #endif
        return -1;
    }
    return 0;
}

#ifdef DEBUG_UART
    void printBuffer(Buffer_t* buffer){
        uint8_t* buff = (uint8_t*) buffer->buffer;
        printf("Buffer: \n\tsize:\n\t\t%d\n\tpayload:\n\t\t", buffer->size);

        for(uint16_t i=0; i< buffer->size; i++){
            printf("%2X ", buff[i]);
            if(!((i+1) % 32)) printf("\n\t\t"); 
        }
        printf("\n");
    }
#endif

#ifdef  PRINT_ERROR
    static inline void report_error(const char* func_name){
        fprintf(stderr, "%s: %s\n", func_name, strerror(errno));
    }
#endif

static inline double getTime_us(){
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts.tv_sec*1E06+ts.tv_nsec*1E-03;
}