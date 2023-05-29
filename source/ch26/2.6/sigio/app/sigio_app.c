#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include <signal.h>

#define DEVICE_FILENAME  "/dev/sigio"

int        dev;

//-------------------------------------------------------------------------------
// ���� : ������ ������ ���μ����� �ٽ� �����Ѵ�.
// �Ű� : ����
// ��ȯ : ����
//-------------------------------------------------------------------------------
static void sigio_handler( int signo )
{
    char  buff[128];
    read( dev, buff, 1 );
    printf( "SIGIO EVENT [%c]\n", buff[0] );
}

int main()
{
    struct sigaction sigact, oldact;
    int    oflag;


    sigact.sa_handler = sigio_handler;
    sigemptyset( &sigact.sa_mask );
    sigact.sa_flags = SA_INTERRUPT;
    if( sigaction( SIGIO, &sigact,&oldact ) < 0 )
    {
        perror( "sigaction error : " );
        exit(0);
    }
    
    dev = open( DEVICE_FILENAME, O_RDWR );
    if( dev >= 0 )
    {
        fcntl( dev, F_SETOWN,getpid() );
        oflag = fcntl( dev, F_GETFL );
        fcntl( dev, F_SETFL, oflag | FASYNC );
        while(1) ;                 
        close(dev);
    }

    return 0;
}


