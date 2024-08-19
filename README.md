# USBGuard - USB Storage Encryption Layer (Kernel Module)

This project implements a block-level encryption layer for USB storage devices using a Linux kernel module. It allows users to securely store and retrieve data from USB devices with block-level encryption. The module encrypts data before writing to the device and decrypts it when reading, ensuring data security without affecting usability. The encryption uses AES-256 with PBKDF2 for key derivation and generates initialization vectors (IVs) based on the block index.

## Implementation Details

The module works by intercepting block I/O operations and applying AES-256 encryption or decryption on data. The key for AES is derived using the PBKDF2 algorithm with a user-provided password. The initialization vector (IV) is generated based on the block index and part of the key to ensure unique IVs for each block. The module provides two key functionalities:

- **Encrypted Read (`BREAD`)**: Reads encrypted data from the USB device, decrypts it using the derived key and block-based IV, and returns the plaintext data.
- **Encrypted Write (`BWRITE`)**: Takes plaintext data, encrypts it using AES with the derived key and block-based IV, and writes the encrypted data to the USB device.

The user must provide a password with every read or write operation. The password is used to derive the AES encryption key using PBKDF2, making the data securely tied to the password.

## Installation

To set up Linux and run this kernel module, follow the steps below:

1. **Create a Virtual Machine**:

   - Use VirtualBox or UTM (for M1 Macs) to create a 64-bit virtual machine.
   - Ensure the VM has at least 50GB of virtual disk space and as many CPU cores as possible.
   - Install Ubuntu 22.04.3 LTS on the VM.

2. **Install Required Dependencies**:

   - Inside the VM, install the necessary dependencies to build the kernel and the module:
     ```bash
     sudo apt-get update
     sudo apt-get install build-essential git fakeroot ncurses-dev xz-utils libssl-dev bc flex libelf-dev bison
     ```

3. **Compile and Install a New Kernel**:

   - Download and uncompress the latest stable kernel (e.g., 6.6.9):

     ```bash
     wget https://mirrors.edge.kernel.org/pub/linux/kernel/v6.x/linux-6.6.9.tar.gz
     tar -xvf linux-6.6.9.tar.gz
     cd linux-6.6.9
     ```

   - Copy the current kernel configuration:

     ```bash
     cp /boot/config-$(uname -r) .config
     make oldconfig
     ```

   - Disable conflicting security keys:

     ```bash
     scripts/config --disable SYSTEM_TRUSTED_KEYS
     scripts/config --disable SYSTEM_REVOCATION_KEYS
     ```

   - Build and install the kernel:
     ```bash
     make -j$(nproc)
     sudo make modules_install
     sudo make install
     ```

4. **Update GRUB and Reboot**:

   - Update the GRUB bootloader:

     ```bash
     sudo update-grub
     ```

   - Reboot the system and select the new kernel:
     ```bash
     sudo reboot
     ```

5. **Build and Load the Kernel Module**:

   - After booting into the new kernel, navigate to the project directory and compile the kernel module:
     ```bash
     make
     sudo insmod usb_encryption.ko
     ```

6. **Verify the Module is Loaded**:
   - You can verify that the module is loaded by using:
     ```bash
     lsmod | grep usb_encryption
     ```

## Usage

The kernel module provides `read` and `write` operations with encryption for USB storage devices. Users must provide a password each time they perform these operations, which is used to derive a key using PBKDF2.

<details>
<summary><strong>Example: How to Use the Module</strong></summary>

```c
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define BLOCK_SIZE 512
#define PASSWORD_MAX_LEN 64

// Data structure for block read/write operations
struct block_rw_ops {
    char* data;
    unsigned int size;
    char password[PASSWORD_MAX_LEN];  // Add password field for key derivation
};

#define BREAD _IOW('a', 'a', struct block_rw_ops)
#define BWRITE _IOW('a', 'b', struct block_rw_ops)

void bread(int fd, char* buffer, unsigned int size, const char* password) {
    struct block_rw_ops request;
    request.data = buffer;
    request.size = size;
    strncpy(request.password, password, PASSWORD_MAX_LEN);

    // Simulate an ioctl call with a password used for encryption key derivation
    printf("Performing encrypted read...\n");
    if (ioctl(fd, BREAD, &request) < 0) {
        perror("Failed to read data from device");
    }
}

void bwrite(int fd, char* buffer, unsigned int size, const char* password) {
    struct block_rw_ops request;
    request.data = buffer;
    request.size = size;
    strncpy(request.password, password, PASSWORD_MAX_LEN);

    // Simulate an ioctl call with a password used for encryption key derivation
    printf("Performing encrypted write...\n");
    if (ioctl(fd, BWRITE, &request) < 0) {
        perror("Failed to write data to device");
    }
}

int main() {
    int fd = open("/dev/kmod", O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return -1;
    }

    char password[PASSWORD_MAX_LEN] = "my_secure_password";  // User-provided password for key derivation
    char write_data[BLOCK_SIZE] = "This is a test data block.";
    char read_data[BLOCK_SIZE] = {0};  // Initialize read buffer

    // Write encrypted data to the USB device
    bwrite(fd, write_data, BLOCK_SIZE, password);

    // Read encrypted data from the USB device
    bread(fd, read_data, BLOCK_SIZE, password);

    printf("Decrypted data: %s\n", read_data);

    close(fd);
    return 0;
}
```

</details>

## Contact

For any inquiries or issues, please contact:

**Name**: Ilia Rubashkin  
**Email**: rubiliam2003@gmail.com
