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

task main()
{
    EV3Init();
    SetSensorTouch(S1);
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
    
    //Read and Display Values
    //Black
    while(EnterButtonState() == NOTPRESSED);
    LBLACK = SENSOR_2;
    RBLACK = SENSOR_3;
    TextNumOut(0, LCD_LINE1, "LBlck: ", LBLACK, 15);
    TextNumOut(0, LCD_LINE2, "RBlck: ", RBLACK, 15);
    Wait(500); //Threshold
    
    //Green
    while(EnterButtonState() == NOTPRESSED);
    LGREEN = SENSOR_2;
    RGREEN = SENSOR_3;
    TextNumOut(0, LCD_LINE1, "LGrn: ", LGREEN, 15);
    TextNumOut(0, LCD_LINE2, "RGrn: ", RGREEN, 15);
    Wait(500);
    
    //Yellow
    while(EnterButtonState() == NOTPRESSED);
    LYELLOW = SENSOR_2;
    RYELLOW = SENSOR_3;
    TextNumOut(0, LCD_LINE1, "LYlw: ", LYELLOW, 15);
    TextNumOut(0, LCD_LINE2, "RYlw: ", RYELLOW, 15);
    Wait(500);
    
    //Calc. calibration values
    while(EnterButtonState() == NOTPRESSED);
    LBG = (LBLACK + LGREEN) / 2;
    RBG = (RBLACK + RGREEN) / 2;
    LGY = (LGREEN + LYELLOW) / 2;
    RGY = (RGREEN + RYELLOW) / 2;
    TextNumOut(0, LCD_LINE1, "LBG: ", LBG, 15);
    TextNumOut(0, LCD_LINE2, "RBG: ", RBG, 15);
    TextNumOut(0, LCD_LINE3, "LGY: ", LGY, 15);
    TextNumOut(0, LCD_LINE4, "RGY: ", RGY, 15);
    Wait(500);
    
    //LBG = 10;
    //RBG = 10;
    //LGY = 100;
    //RGY = 100;
    
    //Line Follower
    while(EnterButtonState() == NOTPRESSED);
    while(SENSOR_2 < LGY || SENSOR_3 < RGY)
    while(true)
    {
        TextNumOut(0, LCD_LINE1, "Tch : ", SENSOR_1, 15);
        TextNumOut(0, LCD_LINE2, "LCol: ", SENSOR_2, 15);
        TextNumOut(0, LCD_LINE3, "RCol: ", SENSOR_3, 15);
        TextNumOut(0, LCD_LINE4, "US  : ", SENSOR_4, 15);
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
        Wait(1);
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
