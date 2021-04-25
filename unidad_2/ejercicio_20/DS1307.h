#ifndef DS1307_h
#define DS1307_h

#include "Arduino.h"

//Hecho por Edisson Penagos 20/04/2021

class DS1307
{
    public:
        DS1307(uint8_t *info);
        bool SetClock(uint8_t second,uint8_t minute, uint8_t hour, uint8_t wday, uint8_t day, uint8_t month, uint8_t year);
        bool ReadClock(uint8_t *info);
        uint8_t bin2bcd(uint8_t bin);
        uint8_t bcd2bin(uint8_t bcd);
        uint8_t *_info;
};


#endif