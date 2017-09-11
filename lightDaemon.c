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
#include <signal.h>
#include "lightDaemon.h"
#include "i2cWrite.h"
#include "leds.h"
#include "qipc.h"

int running=0;
int recvFlag=0;
int i2cNum = -1;
struct leds* recvBuffer;
struct leds runState;
messageID mID;

int speed_tab[11]={
    0,
    1805*1000,
    1610*1000,
    1415*1000,
    1220*1000,
    1025*1000,
    830*1000,
    635*1000,
    440*1000,
    150*1000,
    50*1000,
};

int main(void){
    int ret;
    void* state;
    pthread_t recvThread=0;
    pthread_t thread=0;
    ret = recvInit(&mID);
    if (ret < 0) {
        printf("init recvMessage process err!\n");
        return -1;
    }
    ret = i2cInit(&i2cNum);
    if (ret < 0) {
        printf("init i2c process err!\n");
	return -1;
    }
    ledsInit();
    //init_mutex();
    recvBuffer = (struct leds*)malloc(sizeof(struct leds));
    ret=pthread_create(&recvThread, NULL, pthread_recv_action, NULL);
    pthread_detach(recvThread);
    signal(SIGINT, sigint_handler);
    while (!running) {
    if (1 == recvFlag) {
    if (thread == 0) {
        //ret = pthread_mutex_lock(g_mutex);
        //if (ret!=0) perror("pthread_mutex_lock");
        //strncpy(&runState,recvBuffer,sizeof(struct leds));
        //ret=pthread_mutex_unlock(g_mutex);
        //if(ret!=0) perror("pthread_mutex_unlock");
        ret=pthread_create(&thread, NULL, pthread_action, &runState);
        if (ret != 0) printf("pthread_create err!\n");
        //pthread_detach(thread);
    } else {
        ret = pthread_cancel(thread);
        if (ret != 0) printf("pthread_cancel err!\n");
        //usleep(100);
        ret = pthread_join(thread, &state);
        if (ret != 0) printf("pthread_join err:%d!\n",ret);
        if (state != PTHREAD_CANCELED)
            printf("Thread terminated warning!!!;\n");
        //ret = pthread_mutex_lock(g_mutex);
        //if (ret!=0) perror("pthread_mutex_lock");
        //strncpy(&runState,recvBuffer,sizeof(struct leds));
        //ret=pthread_mutex_unlock(g_mutex);
        //if(ret!=0) perror("pthread_mutex_unlock");
        ret=pthread_create(&thread, NULL, pthread_action, &runState);
        if (ret != 0) printf("pthread_create err!\n");
        //pthread_detach(thread);
    }
    //ret=pthread_mutex_unlock(g_mutex);
    //if(ret != 0){
    //    perror("pthread_mutex_unlock");
    //}
    recvFlag=0;
    }else usleep(100*1000);
    }
    recvRelease(&mID);
    i2cRelease(&i2cNum);
    free(recvBuffer);
    recvBuffer=NULL;
    return 0;
}

void* pthread_recv_action(void* p){
    int recvlen;
    while (1) {
    memset(recvBuffer,'\0',sizeof(struct leds));
    //ret = pthread_mutex_lock(g_mutex);
    //if (ret!=0) perror("pthread_mutex_lock");
    recvlen = recvMessage(&mID, recvBuffer, attr.mq_msgsize);
    if (recvlen < 0) {
    //ret=pthread_mutex_unlock(g_mutex);
    //if(ret!=0) perror("pthread_mutex_unlock");
        continue;
    } else {
        if (recvFlag == 0) {
        runState=*recvBuffer;
        recvFlag=1;
        }
    }
    }
}

void sigint_handler(int sig) {
    running = 1;
}

static void cleanup_handler(void *arg)
{
    //int ret;
    //printf("Called clean-up handler\n");
    //ret=pthread_mutex_unlock(g_mutex);
    //if(ret!=0) perror("pthread_mutex_unlock");
    multipleLight(0,0,0);
}


void* pthread_action(void* p){
    int ret;
    int i=0;
    int j=0;
    struct leds* mrunState=(struct leds*)p;
    //ret = pthread_mutex_lock(g_mutex);
    //if (ret!=0) perror("pthread_mutex_lock");
    //strncpy(&mrunState,(struct leds*)p,sizeof(struct leds));
    //ret=pthread_mutex_unlock(g_mutex);
    //if(ret!=0) perror("pthread_mutex_unlock");

    //pthread_cleanup_push(pthread_mutex_unlock,(void*)g_mutex);
    pthread_cleanup_push(cleanup_handler,NULL);
    while (1) {
    //ret = pthread_mutex_lock(g_mutex);
    //if (ret!=0) perror("pthread_mutex_lock");
#if 0
    printf("num=%d,currentNum=%d,currentColour=%d,speed=%d,direction=%d,mode=%d\n",
        mrunState->num,
        mrunState->currentNum,
        mrunState->currentColour,
        mrunState->speed,
        mrunState->direction,
        mrunState->mode);
#endif
    //sleep(1000);

    switch (mrunState->mode) {
        case _STATIC :
            multipleLight(mrunState->num,
            mrunState->currentNum,
            mrunState->currentColour);
            sleep(1000);
        break;
        case _MOVE :
            if (mrunState->speed > 10) {
                multipleLight(LED_NUM,
                mrunState->currentNum,
                mrunState->currentColour);
                sleep(1000);
            } else if (mrunState->speed <= 0) {
                multipleLight(0,
                mrunState->currentNum,
                mrunState->currentColour);
                sleep(1000);
            } else {
                if (mrunState->direction == _POSITIVE) {
                    for (i=1; i < LED_NUM+1; i++) {
                        multipleLight(mrunState->num,
                        LED_NUM+1-i,
                        mrunState->currentColour);
                        usleep(speed_tab[mrunState->speed]);
                    }
                } else if (mrunState->direction == _REVERSE) {
                    for (i=1; i<LED_NUM+1; i++) {
                        multipleLight(mrunState->num,
                        i,
                        mrunState->currentColour);
                        usleep(speed_tab[mrunState->speed]);
                    }
                } else if (mrunState->direction == _SKIP) {
                    for (i=0; i<LED_NUM+1; i++) {
                        multipleLight(i,
                        1,
                        mrunState->currentColour);
                        usleep(speed_tab[mrunState->speed]);
                    }
                } else {
                    multipleLight(0,
                    mrunState->currentNum,
                    mrunState->currentColour);
                    sleep(1000);
                }
            }
        break;
        case _CLOSE :
            multipleLight(0,
            mrunState->currentNum,
            mrunState->currentColour);
            sleep(1000);
        break;
        case _OTHER :
            if (mrunState->speed > 10) {
                multipleLight(LED_NUM,
                mrunState->currentNum,
                mrunState->currentColour);
                sleep(1000);
            } else if (mrunState->speed <= 0) {
                multipleLight(0,
                mrunState->currentNum,
                mrunState->currentColour);
                sleep(1000);
            } else {
                if (mrunState->direction == _POSITIVE) {
                    multipleLight(1,1,mrunState->currentColour);
                    usleep(speed_tab[mrunState->speed]);
                    for (i=3,j=LED_NUM;i<LED_NUM;i+=2,j--) {
                        multipleLight(i,j,mrunState->currentColour);
                        usleep(speed_tab[mrunState->speed]);
                    }
                    multipleLight(LED_NUM,j,mrunState->currentColour);
                    usleep(speed_tab[mrunState->speed]);
                } else if (mrunState->direction == _REVERSE) {
                    for (i=1,j=(LED_NUM+2)/2;i<LED_NUM;i+=2,j--) {
                        multipleLight(i,j,mrunState->currentColour);
                        usleep(speed_tab[mrunState->speed]);
                    }
                    multipleLight(LED_NUM,j,mrunState->currentColour);
                    usleep(speed_tab[mrunState->speed]);
                } else {
                    multipleLight(mrunState->num,
                        mrunState->currentNum,
                        mrunState->currentColour
                    );
                    usleep(speed_tab[mrunState->speed]);
                    multipleLight(0,
                        mrunState->currentNum,
                        mrunState->currentColour
                    );
                    usleep(speed_tab[mrunState->speed]);
                }
            }
        break;
        default:
            multipleLight(0,
            mrunState->currentNum,
            mrunState->currentColour);
            sleep(1000);
        break;
    }
    //ret=pthread_mutex_unlock(g_mutex);
    //if(ret!=0) perror("pthread_mutex_unlock");
    }
    pthread_cleanup_pop(0);
    return NULL;
}

void init_mutex(void){
    int ret;
    g_mutex = (pthread_mutex_t*)mmap(NULL, sizeof(pthread_mutex_t), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    if ( MAP_FAILED == g_mutex ) {
        perror("mmap");
        exit(1);
    }
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    ret = pthread_mutexattr_setpshared(&attr,PTHREAD_PROCESS_SHARED);
    if ( ret != 0 ) {
        perror("init_mutex pthread_mutexattr_setpshared");
        exit(1);
    }
    pthread_mutex_init(g_mutex, &attr);
}
