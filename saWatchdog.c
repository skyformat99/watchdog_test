#include <stdio.h>
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <asm/types.h>
#include <linux/watchdog.h>
#include <sys/stat.h>
#include <signal.h>

int fd;

void catch_int(int signum)
{
    signal(SIGINT, catch_int);

    printf("In signal handler\n");
    if(0 != close(fd))
        printf("Close failed in signal handler\n");
    else
        printf("Close succeeded in signal handler\n");
}

int main(int argc, const char *argv[])
{
    int sleep_time = atoi(argv[1]);

    int data = 0;
    int ret_val;

    signal(SIGINT, catch_int);

    fd=open("/dev/watchdog",O_WRONLY);
    if (fd==-1)
    {
        perror("watchdog");
        return 1;
    }

    ret_val = ioctl (fd, WDIOC_GETTIMEOUT, &data);
    if (ret_val)
    {
        printf ("\nWatchdog Timer : WDIOC_GETTIMEOUT failed");
    }
    else
    {
        printf ("\nCurrent timeout value is : %d seconds\n", data);
    }

    data = 10;

    ret_val = ioctl (fd, WDIOC_SETTIMEOUT, &data);
    if (ret_val)
    {
        printf ("\nWatchdog Timer : WDIOC_SETTIMEOUT failed");
    }
    else
    {
        printf ("\nNew timeout value is : %d seconds", data);
    }

    ret_val = ioctl (fd, WDIOC_GETTIMEOUT, &data);
    if (ret_val)
    {
        printf ("\nWatchdog Timer : WDIOC_GETTIMEOUT failed");
    }
    else
    {
        printf ("\nCurrent timeout value is : %d seconds\n", data);
    }

    while(1)
    {
        if (1 != write(fd, "\0", 1))
        {
            printf("Write failed\n");
            break;
        }
        else
            printf("Write succeeded\n");

        sleep(sleep_time);
    }

    if (0 != close (fd))
        printf("Close failed\n");
    else
        printf("Close succeeded\n");

    return 0;
}
