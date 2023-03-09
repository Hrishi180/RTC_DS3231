#include<iostream>
#include"drivers.h"
#include"I2CDevice.h"
#include<fstream>
#include<unistd.h>




using namespace std;


int main()
{
    //create a file in write mode called "Readout"
    ofstream fout ("readout" ios_base::app);

    //create object for TIME_DATE class
    TIME_DATE d;

    //create object for I2CDevice class
    EE513::I2CDevice k(1, DEVICE_ADDRESS);

    k.open();
    
    cout <<"----------------RTC_TIME------------------" <<endl;
    d.readtime_date(&k);    
    
    cout <<"------------------TEMP--------------------" <<endl;
    d.readtemp(&k);
    
    cout <<"--------------Current Time----------------" <<endl;
    d.gettimedate();
    
    // write the current time and date value in the file
    fout<<"Time and Date: "<<endl;
    fout << d.curr_hour <<":" << d.curr_min <<":" << d.curr_sec <<endl;
    fout << d.curr_day << ":" << d.curr_mon <<":" << d.curr_year <<endl;
    
    //write the current time value in RTC
    d.writetime(&k);
    
    //Write the current date value in RTC
    d.wrtiedate(&k);
    

    cout<<"---------Alarm_1 after 1 min---------------"<<endl;
    d.setalarm_1(&k);

    
    //delay of 1 min
    usleep(60000000);

    int FLAG = k.readRegister(ALM_FLAG_ADDRESS);


     while(FLAG & 0x01)
    {

        fout << "Alarm 1 triggered at: " << d.curr_hour << ":" <<d.curr_min+1 << ":" << d.curr_sec <<endl;
        k.writeRegister(0x0F, FLAG & ~0x01);

        cout <<"-------------Squarewave generated------------ "<<endl;
        // Generate square wave after Alarm 1 is triggered 
        d.squarewave(&k);

        //delay of 1 min
        usleep(60000000);

         break;

    }
    
    cout<<"---------Alarm_2 after 1 min---------------"<<endl;
    d.setalarm_2(&k);  

    //delay of 1 min 
    usleep(60000000);
    fout << "Alarm 2 triggered at: " << d.curr_hour << ":" <<d.curr_min+3 << ":" << d.curr_sec <<endl;

    // Generate square wave after Alarm 2 is triggered 
    d.squarewave(&k);
    

}
