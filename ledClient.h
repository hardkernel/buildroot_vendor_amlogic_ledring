#ifndef __CLIENT_H
#define __CLIENT_H

#ifdef __cplusplus
extern "C" {
#endif

enum _COLOUR{_RED=0,_GREEN,_BLUE};
enum _DIRECTION{_POSITIVE=0,_REVERSE,_SKIP};
enum _REFRESH_MODE{_STATIC=0,_MOVE,_CLOSE,_OTHER};

struct leds{
    int num;
    int currentNum;
    int speed;  /*0~10*/
    int times;  /*Ns*/
    enum _COLOUR currentColour;
    enum _DIRECTION direction;
    enum _REFRESH_MODE mode;
};

int ledInit(void);
int ledShow(struct leds* state);
int ledRelease(void);

#ifdef __cplusplus
}
#endif
#endif

