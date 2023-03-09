#include"drivers.h"
#include"I2CDevice.h"
#include<iostream>
#include<ctime>
#include<fstream>

using namespace std;


TIME_DATE::TIME_DATE()
{

}

//function to convert hex values to decimal values
int TIME_DATE::HexToDec(char bcd) 
{
return ((bcd/16)*10 + (bcd%16));
}

//function to read time from the RTC
void TIME_DATE::readtime_date(I2CDevice* a)
{
    int sec, min, hours;
    int year, mon, day;

    sec = HexToDec(a->readRegister(SEC_REG_ADDRESS));
    min = HexToDec(a->readRegister(MIN_REG_ADDRESS));
    hours = HexToDec(a->readRegister(HOUR_REG_ADDRESS));

    year = HexToDec(a->readRegister(YEAR_REG_ADDRESS));
    mon = HexToDec(a->readRegister(MONTH_REG_ADDRESS));
    day = HexToDec(a->readRegister(DATE_REG_ADDRESS));

    cout <<"TIME: " << hours << " : " <<min << " : " << sec <<endl;

    cout << "DATE: " << day << " : " <<mon << " : " << year <<endl;  

    
}


//function to read temp value from RTC 
void TIME_DATE::readtemp(I2CDevice *b)
{
    
    int high = b->readRegister(TEMP_MSB_ADDRESS);
    int low = b->readRegister(TEMP_LSB_ADDRESS);

    int temperature;

    temperature = ((high & 0x7f) << 8) | low ;
    temperature =  temperature/256.0;

    cout << " Temperature: " << temperature <<"degree celsius"<<endl;

}

//function to get the current time and date
void TIME_DATE::gettimedate()
{

    time_t curr_time = time(0);

   tm *local_time = localtime(&curr_time);

   
   cout << "Year:" << 1900 + local_time->tm_year<<endl;
   cout << "Month: "<< 1 + local_time->tm_mon<< endl;
   cout << "Day: "<<  local_time->tm_mday << endl;
   cout << "Time: "<< local_time->tm_hour << ":";
   cout << local_time->tm_min << ":";
   cout << local_time->tm_sec << endl;
   

   curr_year = local_time->tm_year%100;
   curr_mon = local_time->tm_mon+1;
   curr_day = local_time->tm_mday;
   curr_hour = local_time->tm_hour;
   curr_min = local_time->tm_min;
   curr_sec = local_time->tm_sec;


}

//function to write the time values into the RTC
void TIME_DATE::writetime(I2CDevice *c)
{
    int hour, min, sec;

    hour = curr_hour;
    min = curr_min;
    sec = curr_sec;

    char in_hour = ((( hour/10) << 4 ) & 0b01110000) | ((hour % 10) & 0b00001111);
    c->writeRegister(HOUR_REG_ADDRESS,in_hour);

    char in_min = ((( min/10) <<4 ) & 0b01110000) | ((min %10) & 0b00001111);
    c->writeRegister(MIN_REG_ADDRESS, in_min);

    char in_sec = ((( sec/10 ) << 4) & 0b01110000) | ((sec % 10) & 0b00001111);
    c->writeRegister(SEC_REG_ADDRESS, in_sec); 
}

//function to write the date values into the RTC
void TIME_DATE::wrtiedate(I2CDevice *d)
{

    int year, month, day;
    year = curr_year;
    month = curr_mon;
    day = curr_day;
    char in_year = (((year/10)<<4) & 0b01110000) | ((year%10) & 0b00001111);
    d->writeRegister(YEAR_REG_ADDRESS, in_year);

    char in_mon = ((( month/10) << 4) & 0b01110000) | ((month %10) & 0b00001111);
    d->writeRegister(MONTH_REG_ADDRESS, in_mon);

    char in_day = (((day/10) << 4) & 0b01110000) | ((day%10) & 0b00001111);
    d->writeRegister(DATE_REG_ADDRESS, in_day);

}

//function to generate the squarewave
void TIME_DATE::squarewave (I2CDevice *e)
{
    int wave = 0;

    e->writeRegister(CONTROL_ADDRESS,wave);
}

//function to configure alarm 1
void TIME_DATE::setalarm_1(I2CDevice *f)
{
    cout <<" alarm called"<<endl;
   //reset alarm flags
   f->writeRegister(ALM_FLAG_ADDRESS, 0x00);

   // store the sec value into alarm 1 sec register
    char in_sec = (((curr_sec/10) << 4) & 0b01110000) | ((curr_sec%10) & 0b00001111);
    f->writeRegister(ALM_1_SEC_ADDRESS, in_sec);

    //increment min by 1 min so that the alarm 1 generates after 1 min
    int min = curr_min +1;

    // store incremented min value into the alarm 1 min register 
    char in_min = (((min/10) << 4) & 0b01110000) | ((min%10) & 0b00001111);
    f->writeRegister(ALM_1_MIN_ADDRESS, in_min);

    // store the hour value into alarm 1 register
    char in_hour = (((curr_hour/10) << 4) & 0b01110000) | ((curr_hour%10) & 0b00001111);
    f->writeRegister(ALM_1_HOUR_ADDRESS,in_hour );

    
    // to set alarm when hours, min and sec match
    char AIM4 = ((8 << 4) & 0b11110000);
    f->writeRegister(ALM_1_DAY_DATE_ADDRESS,AIM4);

    //set bit 0 and 2 high to enable intrupt for alarm1
    char alm_1_en = 5;
    f->writeRegister(CONTROL_ADDRESS, alm_1_en);
    
}

//function to configure alarm 2
void TIME_DATE::setalarm_2(I2CDevice *f)
{
   //reset alarm flags
   f->writeRegister(ALM_FLAG_ADDRESS, 0x00);

  
   //increment min by 3 min so that the alarm 1 generates after 2 min of the first alarm
   int min = curr_min +3;

   // store incremented min value into the alarm 1 min register 
   char in_min = (((min/10) << 4) & 0b01110000) | ((min%10) & 0b00001111);
   f->writeRegister(ALM_2_MIN_ADDRESS, in_min);
    
    // to set alarm2 when min match

   char A2M4 = ((8 << 4) & 0b11110000);
   f->writeRegister(ALM_2_DAY_DATE_ADDRESS,A2M4);

   char A2M3 = ((8 << 4) & 0b11110000);
   f->writeRegister(ALM_2_HOUR_ADDRESS,A2M3);


   //set bit 1 and 2 high to enable intrupt for alarm2
   char alm_1_en = 6;
   f->writeRegister(CONTROL_ADDRESS, alm_1_en);
}