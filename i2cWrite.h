#ifndef __IIC_WRITE
#define __IIC_WRITE

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/i2c.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <asm-generic/errno.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define I2C_M_WR 0
#define I2c_M_RD 1

#define I2C_DEVICE "/dev/i2c-0"
#define I2C_DEVICE_ADDR 0x1F

extern int i2cInit(int* i2cID);
extern int i2cRelease(int* i2cID);
extern int i2cWriteNbyte(int* i2cID, int dev_addr, int slave_addr, int len, unsigned char data_buf[]);

#ifdef __cplusplus
}
#endif

#endif
