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
    while(1)
    {
        
        TextNumOut(0, dLcdGetFontHeight(NORMAL_FONT) * 0, "Tch : ", SENSOR_1, 15);
        TextNumOut(0, dLcdGetFontHeight(NORMAL_FONT) * 1, "LCol: ", SENSOR_2, 15);
        TextNumOut(0, dLcdGetFontHeight(NORMAL_FONT) * 2, "RCol: ", SENSOR_3, 15);
        TextNumOut(0, dLcdGetFontHeight(NORMAL_FONT) * 3, "US  : ", SENSOR_4, 15);
        //usleep(10000);
    }
    EV3Exit();
    return (EXIT_SUCCESS);
}
