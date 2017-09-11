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
#include "clientTest.h"

messageID mID;

/*
enum _COLOUR{_RED=0,_GREEN,_BLUE};
enum _DIRECTION{_CLOCLWISE=0,_COUNTERCLOCKWISE};
enum _REFRESH_MODE{_STATIC=0,_MOVE,_CLOSE,_OTHER};

struct leds{
    int num;
    int currentNum;
    int speed;	//0~10
    enum _COLOUR currentColour;
    enum _DIRECTION direction;
    enum _REFRESH_MODE mode;
};
*/


static struct leds state[]={
    {1,1,1,_RED,_POSITIVE,_MOVE},
    {6,1,6,_RED,_SKIP,_MOVE},
    {2,2,9,_RED,_POSITIVE,_MOVE},
    {6,6,6,_RED,_SKIP,_OTHER},
    {2,2,9,_RED,_POSITIVE,_MOVE},
    {6,6,6,_RED,_REVERSE,_OTHER},
    {6,6,6,_RED,_POSITIVE,_OTHER},
    {6,6,6,_RED,_SKIP,_OTHER},
    {3,4,6,_RED,_SKIP,_OTHER},
};

int main(int argc,char* argv[]){
    int res=0;
    int i=0;
    if (argc == 2 && (
       (strcmp(argv[1], "-h") == 0) ||
       (strcmp(argv[1], "-l") == 0) ||
       (strcmp(argv[1], "--help") == 0))) {
       printf("Usage Options:\n");
       printf("   lightClientTest [0] [1] [2] ... [8].\n\n");
       exit(1);
    } else if ( argc == 1) {
       printf("no options found!!! please run: --help\n");
       exit(1);
    }

    res=sendInit(&mID);
    if (res < 0) {
        printf("init snedmessage err!\n");
	return -1;
    }

    sleep(1);
    i=atoi(argv[1]);
    if ( res > 8) {
        return -1;
    }

    res=snedMessage(&mID,&state[i],sizeof(struct leds));
    if (res < 0) {
        printf("sendMessage err!\n");
	return -1;
    }

    res=sendRelease(&mID);
    if (res < 0) {
        printf("sendRelease err!\n");
        return -1;
    }
    return 0;
}

int sendInit(messageID* id){
    *id= mq_open(SLN_IPC_MQ_NAME, O_WRONLY);
    if (*id < 0)
        return -1;
    return 0;
}

int snedMessage(messageID* id,struct leds* message,int size){
    if (mq_send(*id, (const char*)message, size, 0) < 0)
        return -1;
    return 0;
}

int sendRelease(messageID* id){
    int ret;
    ret = mq_close(*id);
    if (ret != 0) return -1;
    return 0;
}
