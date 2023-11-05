#include "ams5600.h"

float startAngle = 0; //starting angle
int magnetStatus = 0; //value of the status register (MD, ML, MH)
TwoWire _wire_ams = TwoWire(0);

extern void check_magnet_presence() {  
    //This function runs in the setup() and it locks the MCU until the magnet is not positioned properly

    //while the magnet is not adjusted to the proper distance - 32: MD = 1
    while((magnetStatus & 32) != 32) {
        magnetStatus = 0; //reset reading

        _wire_ams.beginTransmission(0x36); //connect to the sensor
        _wire_ams.write(0x0B); //figure 21 - register map: Status: MD ML MH
        _wire_ams.endTransmission(); //end transmission
        _wire_ams.requestFrom(0x36, 1); //request from the sensor

        while(_wire_ams.available() == 0); //wait until it becomes available 
        magnetStatus = _wire_ams.read(); //Reading the data after the request

        Serial.print("Magnet status: ");
        Serial.println(magnetStatus, BIN); //print it in binary so you can compare it to the table (fig 21)      
    }      
    
    //Status register output: 0 0 MD ML MH 0 0 0  
    //MH: Too strong magnet - 100111 - DEC: 39 
    //ML: Too weak magnet - 10111 - DEC: 23     
    //MD: OK magnet - 110111 - DEC: 55

    Serial.println("Magnet found!");
    delay(1000);  
}

extern void ams5600_begin(unsigned int SDA, unsigned int SCL) {
    _wire_ams.begin(SDA, SCL); //start i2C  
	_wire_ams.setClock(800000L); //fast clock

    check_magnet_presence(); //check the magnet (blocks until magnet is found)
}

int lowbyte; //raw angle 7:0
word highbyte; //raw angle 7:0 and 11:8
int rawAngle; //final raw angle 
float degAngle; //raw angle in degrees (360/4096 * [value between 0-4095])
extern void read_raw_angle() { 
    //7:0 - bits
    _wire_ams.beginTransmission(0x36); //connect to the sensor
    _wire_ams.write(0x0D); //figure 21 - register map: Raw angle (7:0)
    _wire_ams.endTransmission(); //end transmission
    _wire_ams.requestFrom(0x36, 1); //request from the sensor
    
    while(_wire_ams.available() == 0); //wait until it becomes available 
    lowbyte = _wire_ams.read(); //Reading the data after the request
    
    //11:8 - 4 bits
    _wire_ams.beginTransmission(0x36);
    _wire_ams.write(0x0C); //figure 21 - register map: Raw angle (11:8)
    _wire_ams.endTransmission();
    _wire_ams.requestFrom(0x36, 1);
    
    while(_wire_ams.available() == 0);  
    highbyte = _wire_ams.read();
    
    //4 bits have to be shifted to its proper place as we want to build a 12-bit number
    highbyte = highbyte << 8; //shifting to left
    //What is happening here is the following: The variable is being shifted by 8 bits to the left:
    //Initial value: 00000000|00001111 (word = 16 bits or 2 bytes)
    //Left shifting by eight bits: 00001111|00000000 so, the high byte is filled in
    
    //Finally, we combine (bitwise OR) the two numbers:
    //High: 00001111|00000000
    //Low:  00000000|00001111
    //      -----------------
    //H|L:  00001111|00001111
    rawAngle = highbyte | lowbyte; //int is 16 bits (as well as the word)

    //We need to calculate the angle:
    //12 bit -> 4096 different levels: 360° is divided into 4096 equal parts:
    //360/4096 = 0.087890625
    //Multiply the output of the encoder with 0.087890625
    degAngle = rawAngle * 0.087890625; 
    
    // Serial.print("Deg angle: ");
    // Serial.println(degAngle, 2); //absolute position of the encoder within the 0-360 circle
  
}

// extern float correctedAngle = 0; //tared angle - based on the startup value
float correctedAngle = 0; //tared angle - based on the startup value
extern void correct_angle() {
    //recalculate angle
    correctedAngle = degAngle - startAngle; //this tares the position

    if(correctedAngle < 0) //if the calculated angle is negative, we need to "normalize" it
    {
    correctedAngle = correctedAngle + 360; //correction for negative numbers (i.e. -15 becomes +345)
    }
    else
    {
        //do nothing
    }
    // Serial.print("Corrected angle: ");
    // Serial.println(correctedAngle, 2); //print the corrected/tared angle  
}

int quadrantNumber, previousquadrantNumber; //quadrant IDs
float numberofTurns = 0; //number of turns
float totalAngle = 0; //total absolute angular displacement
extern void check_quadrant() {
    /*
    //Quadrants:
    4  |  1
    ---|---
    3  |  2
    */

    //Quadrant 1
    if(correctedAngle >= 0 && correctedAngle <=90)
    {
        quadrantNumber = 1;
    }

    //Quadrant 2
    if(correctedAngle > 90 && correctedAngle <=180)
    {
        quadrantNumber = 2;
    }

    //Quadrant 3
    if(correctedAngle > 180 && correctedAngle <=270)
    {
        quadrantNumber = 3;
    }

    //Quadrant 4
    if(correctedAngle > 270 && correctedAngle <360)
    {
        quadrantNumber = 4;
    }
    // Serial.print("Quadrant: ");
    // Serial.println(quadrantNumber); //print our position "quadrant-wise"

    if(quadrantNumber != previousquadrantNumber) //if we changed quadrant
    {
        if(quadrantNumber == 1 && previousquadrantNumber == 4)
        {
        numberofTurns++; // 4 --> 1 transition: CW rotation
        }

        if(quadrantNumber == 4 && previousquadrantNumber == 1)
        {
        numberofTurns--; // 1 --> 4 transition: CCW rotation
        }
        //this could be done between every quadrants so one can count every 1/4th of transition

        previousquadrantNumber = quadrantNumber;  //update to the current quadrant
    
    }  
    // Serial.print("Turns: ");
    // Serial.println(numberofTurns,0); //number of turns in absolute terms (can be negative which indicates CCW turns)  

    //after we have the corrected angle and the turns, we can calculate the total absolute position
    totalAngle = (numberofTurns*360) + correctedAngle; //number of turns (+/-) plus the actual angle within the 0-360 range
    // Serial.print("Total angle: ");
    // Serial.println(totalAngle, 2); //absolute position of the motor expressed in degree angles, 2 digits
}