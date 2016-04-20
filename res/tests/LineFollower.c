/*
 * File:   LineFollower.c
 * Author: acmba
 *
 * Created on 15-apr-2016, 19:23:26
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "libEV3.h"

task main(int argc, char** argv)
{
    EV3Init();
    SetSensorColour(S2);
    SetSensorColour(S3);
    SetSensorUS(S4);
    int LBLACK;
    int RBLACK;
    int LGREEN;
    int RGREEN;
    int LYELLOW;
    int RYELLOW;
    int LBG;
    int RBG;
    int LGY;
    int RGY;
    
    //Read Values
    while(EnterButtonState() == NOTPRESSED);
    LBLACK = SENSOR_2;
    RBLACK = SENSOR_3;
    Wait(500);
    while(EnterButtonState() == NOTPRESSED);
    LGREEN = SENSOR_2;
    RGREEN = SENSOR_3;
    Wait(500);
    while(EnterButtonState() == NOTPRESSED);
    LYELLOW = SENSOR_2;
    RYELLOW = SENSOR_3;
    Wait(500);
    
    //Calc. calibration values
    LBG = (LBLACK + LGREEN) / 2;
    RBG = (RBLACK + RGREEN) / 2;
    LGY = (LGREEN + LYELLOW) / 2;
    RGY = (RGREEN + RYELLOW) / 2;
    
    while(EnterButtonState() == NOTPRESSED);
    
    //Line Follower
    while(SENSOR_2 < LGY || SENSOR_3 < RGY)
    {
        if(SENSOR_2 > LGY)
        {
            OnFwd(OUT_A, -50);
            OnFwd(OUT_D, 90);
        }
        else if(SENSOR_3 > RGY)
        {
            OnFwd(OUT_A, 90);
            OnFwd(OUT_D, -50);
        }
        else if(SENSOR_2 < LBG)
        {
            OnFwd(OUT_A, -50);
            OnFwd(OUT_D, 90);
        }
        else if (SENSOR_3 < RBG)
        {
            OnFwd(OUT_A, 90);
            OnFwd(OUT_D, -50);
        }
        else OnFwd(OUT_AD, 80);
    }
    
    //Vind het blikje
    OnFwd(OUT_D, 100);
    OnFwd(OUT_A, -100);
    Wait(300);
    
    OnFwd(OUT_A, 80);
    OnFwd(OUT_C, -80);
    while(SENSOR_4 > 25);
    Wait(75);
    OnFwd(OUT_AC, 100);
    Wait(2000);
    Off(OUT_AC);
    
    EV3Exit();
    return (EXIT_SUCCESS);
}
