#ifndef __FLOWLIGTH_H
#define __FLOWLIGTH_H

#ifdef __cplusplus
extern "C" {
#endif

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

extern struct mq_attr attr;
extern void init_mutex(void);
pthread_mutex_t* g_mutex;
extern void sigint_handler(int sig);
extern void* pthread_action(void* p);
extern void* pthread_recv_action(void* p);

#ifdef __cplusplus
}
#endif

#endif
