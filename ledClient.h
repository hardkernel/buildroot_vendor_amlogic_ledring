#ifndef __CLIENT_H
#define __CLIENT_H

#include <stdio.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
*  Function: int ledInit(void);
*  Description: leds init function
*  Input:
*  Output:
*  Return: led init state. sucess: 0  fail: -1
*  Others:
**********************************************************************/
int ledInit(void);

/*********************************************************************
*  Function: int ledShow(int num, int speed, int time, int style);
*  Description: leds set action function
*  Input:
*      num: the number of led. range: [1 ~ 6]
*      rgbflag: led color flag. range: 0: signal color 1: rgb color
*      speed : the speed of led move. range: [0 ~ N] (ms)
*      time : the time of led move. range: [0 ~ N] (s)
*      style : the style of led move. range: [0 ~ 5]
*  Output:
*  Return: led set state. sucess: 0  fail: -1
*  Others:
**********************************************************************/
int ledShow(int num, int rgbflag, int speed, int time, int style);

/*********************************************************************
*  Function: int ledRelease(void);
*  Description: leds release function
*  Input:
*  Output:
*  Return: led release state. sucess: 0  fail: -1
*  Others:
**********************************************************************/
int ledRelease(void);

#ifdef __cplusplus
}
#endif
#endif

