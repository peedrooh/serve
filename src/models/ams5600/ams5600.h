#ifndef AMS_5600_H
#define AMS_5600_H
#include <Arduino.h>
#include <Wire.h>

extern void ams5600_begin(unsigned int SDA, unsigned int SCL);
extern void check_magnet_presence();
extern void read_raw_angle();
extern void correct_angle();
extern void check_quadrant();

extern int magnetStatus;
extern float startAngle;
extern int lowbyte;
extern word highbyte;
extern int rawAngle;
extern float degAngle;
extern int quadrantNumber, previousquadrantNumber;
extern float numberofTurns;
extern float totalAngle;
extern float correctedAngle;

#endif