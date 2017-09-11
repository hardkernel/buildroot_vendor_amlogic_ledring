#ifndef __LEDS_H
#define __LEDS_H

#define addr_led_reg0 0   //input data
#define addr_led_reg1 1   //output data
#define addr_led_reg2 2   //inversion
#define addr_led_reg3 3   //direction

#define LED_NUM  6

int ledsInit(void);
void multipleLight(int num,int numflag,int colour);

extern int i2cNum;

#endif
