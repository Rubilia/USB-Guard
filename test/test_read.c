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

    char buffer[512];
    ioctl(fd, IOCTL_READ, buffer);
    printf("Encrypted data: %s\n", buffer);
    
    close(fd);
    return 0;
}
