/*
 * File:   MotorTest.c
 * Author: acmba
 *
 * Created on 9-apr-2016, 12:19:50
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libEV3.h>

int main(int argc, char** argv)
{
    EV3Init();
    SetMotorPolarity(OUT_AD, 0);
    OnFwd(OUT_AD, 50);
    sleep(1);
    OnFwdSync(OUT_AD, 50, 0);
    sleep(10);
    //Off(OUTPUT_AB);
    EV3Exit();
    return (EXIT_SUCCESS);
}