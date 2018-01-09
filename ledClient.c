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
#include <stdio.h>
#include <mqueue.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <string.h>
#include "ledStyle.h"
#include "ledClient.h"
#ifdef _USE_LEDRING_DAEMON
#include "qipc.h"
#endif

#ifdef _USE_LEDRING_DAEMON
static messageID mID;
#else
#define DEV_LEDRING_NAME            "/dev/ledring"
#define DEV_LEDRGB_NAME             "/dev/ledrgb"
#define DEFAULT_SPEED               230

#define CMD_LEDRING_ARG             0x100001
#define CMD_LED_MUTE_ARG            0x100002
#define CMD_LED_LISTENING_ARG       0x100003
#define LED_OFF                     0
#define LED_ON                      1
#define LED_4KEY                    1
#define LED_0KEY                    0

int fp1,fp2;
int keyFlag = -1;
int rgbFlag = -1;

struct leds{
    int num;   /*the number of leds*/
    int times; /*N: stop after action N times, 0: cycle action*/
    int speed; /*the speed of move. [0~N] ms*/
    int time; /*the timeout of move. [0 ~ N] s . 0s mean close timeout*/
    int style[12][12]; /*style data*/
} styleData = {12, 0, DEFAULT_SPEED, 0, {0}};

#endif

int ledInit(void) {
    int ret, mode;
#ifdef _USE_LEDRING_DAEMON
    mID= mq_open(SLN_IPC_MQ_NAME, O_WRONLY);
    if (mID < 0)
        return -1;
#else
    fp1 = open(DEV_LEDRING_NAME, O_RDWR);
    if (fp1 > 0) {
        ret = read(fp1, &mode, sizeof(mode));
        if (-1 == ret) {
            keyFlag = -1;
            printf("read led node fail!\n");
        } else keyFlag = mode;
    } else {
        keyFlag = -1;
        printf("open \"%s\" fail!\n",DEV_LEDRING_NAME);
        return -1;
    }

    fp2 = open(DEV_LEDRGB_NAME, O_RDWR);
    if (fp2 > 0)
        rgbFlag = 1;
    else {
        rgbFlag = -1;
        printf("open \"%s\" fail!\n", DEV_LEDRGB_NAME);
        return -1;
    }
#endif
    return 0;
}

int ledShow(int num, int times, int speed,
    int time, int style, int mute_led, int listen_led){
    int ret = 0;
    int mute_led_state, listen_led_state;

#ifdef _USE_LEDRING_DAEMON
    if (mq_send(mID, (const char*)state, sizeof(struct leds), 0) < 0) {
        printf("send message err!\n");
        return -1;
    }
#else
    styleData.num = num;
    styleData.speed = speed;
    styleData.time = time;
    styleData.times = times;
    mute_led_state = mute_led;
    listen_led_state = listen_led;

    switch (keyFlag) {
        case LED_4KEY:
            ret = ioctl(fp1,CMD_LED_LISTENING_ARG, &mute_led_state);
            ret = ioctl(fp1,CMD_LED_MUTE_ARG, &listen_led_state);
        break;
        case LED_0KEY:
            if ((style > (sizeof(m_style)/sizeof(m_style[0])-1)) || (style <0)) {
                printf("This signal color style is not supported, please check!\n");
                return -1;
            }
            if (num > 6) {
               styleData.num = 6;
               printf("signal color led num out of range,use default num,please check!\n");
            }
            memcpy((char*)&styleData.style, (char*)&m_style[style], sizeof(styleData.style));
            ret = ioctl(fp1,CMD_LEDRING_ARG,&styleData);
        break;
    }

    if (rgbFlag == 1) {
        if (num > 12) {
            printf("rgb color led num out of range, please check!\n");
            return -1;
        }

        if ((style > (sizeof(rgb_style)/sizeof(rgb_style[0])-1)) || (style <0)) {
            printf("This style is not supported, please check!\n");
            return -1;
        }

        memcpy((char*)&styleData.style, (char*)&rgb_style[style], sizeof(styleData.style));
        ret = ioctl(fp2,CMD_LEDRING_ARG,&styleData);
    }
#endif
    return 0;
}

int ledRelease(void) {
    int ret = 0;
#ifdef _USE_LEDRING_DAEMON
    ret = mq_close(mID);
    if (ret != 0) {
        printf("send message release err!\n");
        return -1;
    }
#else
    if (keyFlag != -1) {
        ret = close(fp1);
        if (ret < 0) return -1;
    }
    if (rgbFlag != -1) {
        ret = close(fp2);
        if (ret < 0) return -1;
    }
#endif
    return 0;
}
