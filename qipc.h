#ifndef __IPCC_H
#define __IPCC_H

#include <stdio.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "lightDaemon.h"

#define SLN_IPC_MQ_NAME "/tmp"

#define SLN_IPC_MQ_MAXMSG 1

typedef mqd_t messageID;

extern int recvInit(messageID* id);
extern int recvRelease(messageID* id);
extern int recvMessage(messageID* id, struct leds* message, int size);

#endif
