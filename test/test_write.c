#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "common.h"

int main() {
    int fd = open("/dev/kmod", O_RDWR);
    if (fd < 0) {
        printf("Error opening device\n");
        return -1;
    }

    char data[512] = "Testing encryption layer!";
    ioctl(fd, IOCTL_WRITE, data);
    
    close(fd);
    return 0;
}
