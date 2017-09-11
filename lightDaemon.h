#ifndef __FLOWLIGTH_H
#define __FLOWLIGTH_H

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

enum _COLOUR{_RED=0,_GREEN,_BLUE};
enum _DIRECTION{_POSITIVE=0,_REVERSE,_SKIP};
enum _REFRESH_MODE{_STATIC=0,_MOVE,_CLOSE,_OTHER};

struct leds{
    int num;
    int currentNum;
    int speed;/*0~10*/
    enum _COLOUR currentColour;
    enum _DIRECTION direction;
    enum _REFRESH_MODE mode;
};

extern struct mq_attr attr;
extern void init_mutex(void);
pthread_mutex_t* g_mutex;
extern void sigint_handler(int sig);
extern void* pthread_action(void* p);
extern void* pthread_recv_action(void* p);

#endif
