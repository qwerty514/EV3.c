/*
 * File:   SensorTest.c
 * Author: acmba
 *
 * Created on 9-apr-2016, 21:28:21
 */

#include <stdio.h>
#include <stdlib.h>
#include <libEV3.h>

int main(int argc, char** argv)
{
    EV3Init();
    SetSensorTouch(S1);
    SetSensorColour(S2);
    SetSensorUS(S4);
    while(1)
    {
        NumOut(89, 20, (unsigned char*)(puart->Raw[port][puart->Actual[port]][0]), 5);
        //usleep(10000);
    }
    EV3Exit();
    return (EXIT_SUCCESS);
}
