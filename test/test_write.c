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

    char data[512] = "Testing partial block write";
    unsigned int partial_size = 100;  // Writing only 100 bytes
    ioctl(fd, IOCTL_WRITE, data);
    ioctl(fd, IOCTL_WRITE, &partial_size);
    
    close(fd);
    return 0;
}
