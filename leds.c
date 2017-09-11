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
#include "leds.h"
#include "i2cWrite.h"

int ledsInit(void){
    unsigned char regValue[4]={0x00,0x7F,0x80,0xFF};
    i2cWriteNbyte(&i2cNum,I2C_DEVICE_ADDR,addr_led_reg3,1,&regValue[0]);
    i2cWriteNbyte(&i2cNum,I2C_DEVICE_ADDR,addr_led_reg1,1,&regValue[0]);
    return 0;
}

void multipleLight(int num,int numflag,int colour){
    int i=0;
    unsigned char j=0;
    unsigned char regValue=0;
    if ((numflag < 1) || (numflag > LED_NUM) || (num <= 0) || (num > LED_NUM)) {
        regValue=0;
        i2cWriteNbyte(&i2cNum, I2C_DEVICE_ADDR, addr_led_reg1, 1, &regValue);
        return;
    }
    for (i=0; i < num; i++) {
        j = (j >> 1)|0x80;
    }
    for (i=0; i < numflag-1; i++) {
        j = j >> 1;
        if (j & 0x02) j = j|0x80;
    }
    i2cWriteNbyte(&i2cNum,I2C_DEVICE_ADDR,addr_led_reg1,1,&j);
}

