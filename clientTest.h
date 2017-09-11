#ifndef __CLIENT_H
#define __CLIENT_H

#include <stdio.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include "lightDaemon.h"
#include "qipc.h"

typedef mqd_t messageID;

extern int sendInit(messageID* id);
extern int snedMessage(messageID* id,struct leds* message,int size);
extern int sendRelease(messageID* id);

#endif

