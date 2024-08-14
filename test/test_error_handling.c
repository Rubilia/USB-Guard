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
    // Test wrong password case
    char wrong_password[64] = "incorrect_password";
    if (ioctl(fd, IOCTL_READ, wrong_password) != 0) {
        printf("Error handling test passed: Incorrect password\n");
    } else {
        printf("Error handling test failed\n");
    }
    
    close(fd);
    return 0;
}
