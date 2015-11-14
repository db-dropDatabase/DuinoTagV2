#ifndef MILES_DECODE_H
#define MILES_DECODE_H

#define IR_BURST_UPPER 60 //numbers to turn raw lengths of IR into signal, probably don't mess with them
#define IR_BURST_HEADER 40
#define IR_BURST_ONE 20
#define IR_BURST_ZERO 8

typedef unsigned int myByte;

extern double MHitDelay(myByte in);
extern unsigned int milesHealth(myByte health);
extern unsigned int milesRPM(myByte rpm);
extern unsigned int milesDamage(myByte damageIn);
extern unsigned int decodePulse(int pulseLength);

#endif //guard
