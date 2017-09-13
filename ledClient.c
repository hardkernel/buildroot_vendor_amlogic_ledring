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
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include "ledClient.h"
#include "qipc.h"

static messageID mID;

int ledInit(void){
    mID= mq_open(SLN_IPC_MQ_NAME, O_WRONLY);
    if (mID < 0)
        return -1;
    return 0;
}

int ledShow(struct leds* state) {
    int ret = 0;
    if (mq_send(mID, (const char*)state, sizeof(struct leds), 0) < 0) {
        printf("send message err!\n");
        return -1;
    }
    return 0;
}

int ledRelease(void) {
    int ret = 0;
    ret = mq_close(mID);
    if (ret != 0) {
        printf("send message release err!\n");
        return -1;
    }
    return 0;
}
