/*
 * File:   SensorTest.c
 * Author: acmba
 *
 * Created on 9-apr-2016, 21:28:21
 */

#include <stdio.h>
#include <stdlib.h>
#include <libEV3.h>

task main(int argc, char** argv)
{
    EV3Init();
    SetSensorTouch(S1);
    SetSensorColour(S2);
    SetSensorColour(S3);
    SetSensorUS(S4);
    //SetSensorNXTUS(S4);
    //SetSensorNXTLight(S4);
    //SetColourSensorMode(S2, REFLECTED);
    while(1)
    {
        
        TextNumOut(0, LCD_LINE1, "Tch : ", SENSOR_1, 15);
        TextNumOut(0, LCD_LINE2, "LCol: ", SENSOR_2, 15);
        TextNumOut(0, LCD_LINE3, "RCol: ", SENSOR_3, 15);
        TextNumOut(0, LCD_LINE4, "US  : ", SENSOR_4, 15);
        //usleep(10000);
    }
    EV3Exit();
    return (EXIT_SUCCESS);
}
