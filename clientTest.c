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
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <popt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "ledClient.h"

static const struct option OPTIONS[] =
{
    { "speed", required_argument, NULL, 's' },
    { "rgbflag", required_argument, NULL, 'r' },
    { "time", required_argument, NULL, 't' },
    { "num", required_argument, NULL, 'n' },
    { "style", required_argument, NULL, 'S' },
    { NULL, 0, NULL, 0 },
};
static void printHelp(const char* s_arg) {
    printf("\n");
    printf("Usage: %s [options...]*\n", s_arg);
    printf("\n");
    printf("Options:\n");
    printf("    -h, --help     show this help.\n");
    printf("    -r, --rgbflag  led color flag         --range: 0: signal color 1: rgb color\n");
    printf("    -n, --num      the numbers of led.    --range[0 ~ N]\n");
    printf("    -s, --speed    the speed of move.     --range[0 ~ N] ms\n");
    printf("    -t, --time     the timeout of move.   --range[0 ~ N] s . 0s mean close timeout\n");
    printf("    -S, --style    the style of move.     --range[0 ~ max style]\n");
    printf("\n");
    printf("eg: %s --num=6 --rgbflag=0 --speed=300 --time=5 --style=5\n",s_arg);
    printf("\n");
}

int main(int argc,char* argv[]){
    int res=0;
    int arg;
    char mbuf[100];
    int num, speed, time, style, rgbflag;

    if (argc < 2) {
        printHelp(argv[0]);
        return -1;
    }

    if ((2 == argc) && ((!strcmp(argv[1],"--help")) || (!strcmp(argv[1],"-h")))) {
        printHelp(argv[0]);
        return -1;
   }

   while ((arg = getopt_long(argc, argv, "", OPTIONS, NULL)) != -1) {
        switch (arg) {
        case 'n': num = atoi(optarg); break;
        case 's': speed = atoi(optarg); break;
        case 't': time = atoi(optarg); break;
        case 'S': style = atoi(optarg); break;
        case 'r': rgbflag = atoi(optarg); break;
        case '?':
            sprintf(mbuf,"%d",arg);
            printf("Invalid command argument:%s\n",mbuf);
        return -1;
        }
    }

    res = ledInit();
    if (res < 0) {
        printf("ledInit err!\n");
        return -1;
    }

    res = ledShow(num, rgbflag, speed, time, style);
    if (res < 0) {
        printf("ledShow err!\n");
        return -1;
    }
    res = ledRelease();
    if (res < 0) {
        printf("ledRelease err!\n");
        return -1;
    }

    return 0;
}
