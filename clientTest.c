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

#include "ledClient.h"

static struct leds state[]={
    {1,1,8,0,_RED,_POSITIVE,_STATIC},    //0
    {1,1,8,5,_RED,_POSITIVE,_STATIC},    //1

    {2,1,11,0,_RED,_POSITIVE,_MOVE},     //2
    {2,1,11,5,_RED,_POSITIVE,_MOVE},     //3

    {3,1,0,0,_RED,_POSITIVE,_MOVE},      //4
    {3,1,0,5,_RED,_POSITIVE,_MOVE},      //5

    {4,1,9,0,_RED,_POSITIVE,_MOVE},      //6
    {4,1,9,5,_RED,_POSITIVE,_MOVE},      //7

    {4,1,9,0,_RED,_REVERSE,_MOVE},       //8
    {4,1,9,5,_RED,_REVERSE,_MOVE},       //9

    {4,1,8,0,_RED,_SKIP,_MOVE},          //10
    {4,1,8,5,_RED,_SKIP,_MOVE},          //11

    {5,1,8,0,_RED,_SKIP,_CLOSE},         //12

    {6,1,11,0,_RED,_SKIP,_OTHER},        //13
    {6,1,11,5,_RED,_SKIP,_OTHER},        //14

    {1,1,0,0,_RED,_SKIP,_OTHER},         //15
    {1,1,0,5,_RED,_SKIP,_OTHER},         //16

    {6,1,6,0,_RED,_POSITIVE,_OTHER},     //17
    {6,1,6,5,_RED,_POSITIVE,_OTHER},     //18

    {6,1,6,0,_RED,_REVERSE,_OTHER},      //19
    {6,1,6,5,_RED,_REVERSE,_OTHER},      //20

    {6,1,9,0,_RED,_SKIP,_OTHER},         //21
    {6,1,9,5,_RED,_SKIP,_OTHER},         //22

    {3,1,9,0,_RED,_SKIP,_OTHER},         //23
    {3,1,9,5,_RED,_SKIP,_OTHER},         //24
    //..........
};

int main(int argc,char* argv[]){
    int res=0;
    int i=0;
    if (argc == 2 && (
       (strcmp(argv[1], "-h") == 0) ||
       (strcmp(argv[1], "-l") == 0) ||
       (strcmp(argv[1], "--help") == 0))) {
       printf("Usage Options:\n");
       printf("   lightTest [0] [1] [2] ... [24].\n\n");
       exit(1);
    } else if ( argc == 1) {
       printf("no options found!!! please run: --help\n");
       exit(1);
    }

    i=atoi(argv[1]);
    if ( res > 24) {
        return;
    }

    res = ledInit();
    if (res < 0) {
        printf("ledInit err!\n");
        return -1;
    }
    sleep(1);
    res = ledShow(&state[i]);
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
