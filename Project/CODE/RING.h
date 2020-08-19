#ifndef _RING_H_
#define _RINF_H_

extern uint8 ringstate;
extern uint8 inring_st;
extern uint8 inring_nd;
extern float ring_ratio;
extern uint8 ring_servo;
extern int RMPWM;
extern int count;
extern int angle_ring;
void ringjudge_st();
void ringjudge_nd();
void ring_int();
void ringProcess();
#endif