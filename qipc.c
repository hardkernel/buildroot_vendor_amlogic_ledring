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
#include "qipc.h"
#include "lightDaemon.h"

struct mq_attr setattr={0,SLN_IPC_MQ_MAXMSG,sizeof(struct leds),0};
struct mq_attr attr;

int recvInit(messageID* id) {
    *id = mq_open(SLN_IPC_MQ_NAME, O_RDONLY | O_CREAT | O_EXCL, 0644, &setattr);
    if ((*id < 0) && (errno != EEXIST)) {
        printf("mq_open first: %s\n", strerror(errno));
        return -1;
    }
    if ((*id < 0) && (errno == EEXIST)) {
        *id = mq_open(SLN_IPC_MQ_NAME, O_RDONLY);
        if (*id < 0) {
            printf("mq_open again: %s\n", strerror(errno));
            return -1;
        }
    }
    if (mq_getattr(*id, &attr) < 0) {
        printf("mq_getattr: %s\n", strerror(errno));
        return -1;
    }
    //printf("flags: %ld, maxmsg: %ld, msgsize: %ld, curmsgs: %ld\n",
    //attr.mq_flags, attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
    return 0;
}

int recvMessage(messageID* id,struct leds* message,int size){
    int recvlen = 0;
    recvlen = mq_receive(*id, (char*)message, size, 0);
    return recvlen;
}

int recvRelease(messageID* id){
    mq_close(*id);
    mq_unlink(SLN_IPC_MQ_NAME);
    return 0;
}


