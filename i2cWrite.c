/*
 * This file is part of ledring.
 * Copyright (c) amlogic 2017
 * All rights reserved.
 * author:renjun.xu@amlogic.com
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software.
 *
 */
#include "i2cWrite.h"

int i2cInit(int* i2cID) {
    *i2cID = open(I2C_DEVICE, O_RDWR);
    if (*i2cID < 0) {
        printf("%s, open device file %s error: %s.\n",I2C_DEVICE, __FUNCTION__, strerror(errno));
        return -1;
    }
    return 0;
}

int i2cRelease(int* i2cID){
    if (*i2cID >= 0) {
        close(*i2cID);
        *i2cID = -1;
        return 0;
    }
    return -1;
}

int i2cWriteNbyte(int* i2cID, int dev_addr, int slave_addr, int len, unsigned char data_buf[])
{
    int tmp_ret = 0;
    struct i2c_rdwr_ioctl_data ctl_data;
    struct i2c_msg msg;
    unsigned char msg_buf[52];

    memset((void *) msg_buf, 0, 52);

    msg_buf[0] = (unsigned char) (slave_addr & 0x00ff);

    if (data_buf == NULL) {
        return -1;
    }

    if (len < 50) {
        memcpy((void *) &msg_buf[1], data_buf, len);
    } else {
        printf("I2C_WriteNbyte len(%d) > 50, error!\n", len);
        return -1;
    }

    msg.addr = dev_addr;
    msg.flags = I2C_M_WR;
    msg.len = 1 + len;
    msg.buf = msg_buf;
    ctl_data.nmsgs = 1;
    ctl_data.msgs = &msg;

    tmp_ret = ioctl(*i2cID, I2C_RDWR, &ctl_data);

    usleep(10 * 1000);
    return tmp_ret;
}
