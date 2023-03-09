#ifndef DRIVERS_H_
#define DRIVERS_H_


// Defining addresses 

#define DEVICE_ADDRESS          0x68

#define SEC_REG_ADDRESS         0x00
#define MIN_REG_ADDRESS         0x01
#define HOUR_REG_ADDRESS        0x02

#define DATE_REG_ADDRESS        0x04
#define MONTH_REG_ADDRESS       0x05
#define YEAR_REG_ADDRESS        0x06

#define TEMP_MSB_ADDRESS        0x11
#define TEMP_LSB_ADDRESS        0x12


#define ALM_1_SEC_ADDRESS       0x07
#define ALM_1_MIN_ADDRESS       0x08
#define ALM_1_HOUR_ADDRESS      0x09
#define ALM_1_DAY_DATE_ADDRESS  0x0A
#define ALM_2_MIN_ADDRESS       0x0B
#define ALM_2_HOUR_ADDRESS      0x0C
#define ALM_2_DAY_DATE_ADDRESS  0x0D

#define CONTROL_ADDRESS         0x0E
#define ALM_FLAG_ADDRESS        0x0F 




#include "I2CDevice.h"
#include <iostream>


using namespace std;

//create class TIME_DATE

class TIME_DATE: public EE513::I2CDevice 
{
    public:

    int curr_year, curr_mon, curr_day, curr_hour, curr_min, curr_sec;

     TIME_DATE();
    //function declarations
    void readtime_date(I2CDevice*);
    int HexToDec(char bcd);
    void readtemp(I2CDevice*);
    void gettimedate();
    void writetime(I2CDevice *);
    void wrtiedate(I2CDevice *);
    void squarewave (I2CDevice *);
    void setalarm_1(I2CDevice *);
    void setalarm_2(I2CDevice *);



};

#endif