/* 
 * File:   EV3lib.h
 * Author: Bram Mak
 *
 * Created on 27 mei 2015, 22:39
 */

#ifndef EV3LIB_H
#define	EV3LIB_H

#include "lms2012.h"

typedef unsigned char byte;

//Motor stuffs
//<editor-fold>
#define BRAKE 0x01
#define FLOAT 0x00

#define FORWARD 1
#define BACKWARD -1
#define SWITCH 0

#define MEDIUM 0x08
#define LARGE 0x07

//OUTPUTS
//<editor-fold>
#define OUTPUT_A 0x01
#define OUTPUT_B 0x02
#define OUTPUT_C 0x04
#define OUTPUT_D 0x08

#define OUTPUT_AB 0x03
#define OUTPUT_BA 0x05

#define OUTPUT_AC 0x05
#define OUTPUT_CA 0x05

#define OUTPUT_AD 0x09
#define OUTPUT_DA 0x09

#define OUTPUT_ABC 0x07
#define OUTPUT_ACB 0x07
#define OUTPUT_BAC 0x07
#define OUTPUT_BCA 0x07
#define OUTPUT_CAB 0x07
#define OUTPUT_CBA 0x07

#define OUTPUT_BCD 0x0E
#define OUTPUT_BDC 0x0E
#define OUTPUT_CBD 0x0E
#define OUTPUT_CDB 0x0E
#define OUTPUT_DBC 0x0E
#define OUTPUT_DCB 0x0E

#define OUTPUT_ACD 0x0D
#define OUTPUT_ADC 0x0D
#define OUTPUT_CAD 0x0D
#define OUTPUT_CDA 0x0D
#define OUTPUT_DAC 0x0D
#define OUTPUT_DCA 0x0D

#define OUTPUT_ABD 0x0B
#define OUTPUT_ADB 0x0B
#define OUTPUT_BAD 0x0B
#define OUTPUT_BDA 0x0B
#define OUTPUT_DAB 0x0B
#define OUTPUT_DBA 0x0B

#define OUTPUT_ALL 0x0F
#define OUTPUT_ABCD 0x0F
#define OUTPUT_ABDC 0x0F
#define OUTPUT_ACBD 0x0F
#define OUTPUT_ACDB 0x0F
#define OUTPUT_ADBC 0x0F
#define OUTPUT_ADCB 0x0F
#define OUTPUT_BACD 0x0F
#define OUTPUT_BADC 0x0F
#define OUTPUT_BCAD 0x0F
#define OUTPUT_BCDA 0x0F
#define OUTPUT_BDAC 0x0F
#define OUTPUT_BDCA 0x0F
#define OUTPUT_CABD 0x0F
#define OUTPUT_CADB 0x0F
#define OUTPUT_CBAD 0x0F
#define OUTPUT_CBDA 0x0F
#define OUTPUT_CDAB 0x0F
#define OUTPUT_CDBA 0x0F
#define OUTPUT_DABC 0x0F
#define OUTPUT_DACB 0x0F
#define OUTPUT_DBAC 0x0F
#define OUTPUT_DBCA 0x0F
#define OUTPUT_DCAB 0x0F
#define OUTPUT_DCBA 0x0F

#define OUT_A 0x01
#define OUT_B 0x02
#define OUT_C 0x04
#define OUT_D 0x08

#define OUT_AB 0x03
#define OUT_BA 0x05

#define OUT_AC 0x05
#define OUT_CA 0x05

#define OUT_AD 0x09
#define OUT_DA 0x09

#define OUT_ABC 0x07
#define OUT_ACB 0x07
#define OUT_BAC 0x07
#define OUT_BCA 0x07
#define OUT_CAB 0x07
#define OUT_CBA 0x07

#define OUT_BCD 0x0E
#define OUT_BDC 0x0E
#define OUT_CBD 0x0E
#define OUT_CDB 0x0E
#define OUT_DBC 0x0E
#define OUT_DCB 0x0E

#define OUT_ACD 0x0D
#define OUT_ADC 0x0D
#define OUT_CAD 0x0D
#define OUT_CDA 0x0D
#define OUT_DAC 0x0D
#define OUT_DCA 0x0D

#define OUT_ABD 0x0B
#define OUT_ADB 0x0B
#define OUT_BAD 0x0B
#define OUT_BDA 0x0B
#define OUT_DAB 0x0B
#define OUT_DBA 0x0B

#define OUT_ALL 0x0F
#define OUT_ABCD 0x0F
#define OUT_ABDC 0x0F
#define OUT_ACBD 0x0F
#define OUT_ACDB 0x0F
#define OUT_ADBC 0x0F
#define OUT_ADCB 0x0F
#define OUT_BACD 0x0F
#define OUT_BADC 0x0F
#define OUT_BCAD 0x0F
#define OUT_BCDA 0x0F
#define OUT_BDAC 0x0F
#define OUT_BDCA 0x0F
#define OUT_CABD 0x0F
#define OUT_CADB 0x0F
#define OUT_CBAD 0x0F
#define OUT_CBDA 0x0F
#define OUT_CDAB 0x0F
#define OUT_CDBA 0x0F
#define OUT_DABC 0x0F
#define OUT_DACB 0x0F
#define OUT_DBAC 0x0F
#define OUT_DBCA 0x0F
#define OUT_DCAB 0x0F
#define OUT_DCBA 0x0F
//</editor-fold>
//</editor-fold>

//Tacho stuffs
//<editor-fold>
#define MOTOR1COUNT ptacho[0].TachoCounts
#define MOTOR2COUNT ptacho[1].TachoCounts
#define MOTOR3COUNT ptacho[2].TachoCounts
#define MOTOR4COUNT ptacho[3].TachoCounts
#define MOTOR1SPEED ptacho[0].Speed
#define MOTOR2SPEED ptacho[1].Speed
#define MOTOR3SPEED ptacho[2].Speed
#define MOTOR4SPEED ptacho[3].Speed
#define MOTOR1SENSOR ptacho[0].TachoSensor
#define MOTOR2SENSOR ptacho[1].TachoSensor
#define MOTOR3SENSOR ptacho[2].TachoSensor
#define MOTOR4SENSOR ptacho[3].TachoSensor
extern MOTORDATA *ptacho;
extern int tachofile;
//</editor-fold>

//Sensor-stuffs
//<editor-fold>
#define SENSOR_1 *psensor1
#define SENSOR_2 *psensor2
#define SENSOR_3 *psensor3
#define SENSOR_4 *psensor4
#define S1 0x00
#define S2 0x01
#define S3 0x02
#define S4 0x03
extern void *psensor1;
extern void *psensor2;
extern void *psensor3;
extern void *psensor4;
//</editor-fold>

//LCD stuffs
//<editor-fold>
extern LCD lcd;
extern char lcdupdate;

//Button stuffs
#define PRESSED 1
#define NOTPRESSED 0
extern UI *pui;
//</editor-fold>

//Time
//<editor-fold>
#include <time.h>
#define Wait(time) nanosleep(0, (time*1000000))
//</editor-fold>

//Functions
//<editor-fold>
#ifdef	__cplusplus
extern "C" {
#endif
    void OutputInit();
    void OutputExit();
    void SetMotorPolarity(char outputs, char polarity);
    void SetMotorType(char outputnumber, char type);                            //Outputnumbers are 0-3, do NOT use "OUTPUT_A" for example
    void OnFwd(char outputs, char power);
    void OnFwdSpeed(char outputs, char speed);
    void OnFwdSync(char outputs, char speed, short turn);
    void Off(char outputs);
    void WaitForMotor(char outputs);
    void ResetTacho(char outputs);                                              //ResetTacho and ReadTacho only use d_pwm, no need for TachoInit();
    void ReadTacho(char outputnumber, char *speed, int *count);                 //Outputnumbers are 0-3, do NOT use "OUTPUT_A" for example
    void TachoInit();
    void TachoExit();
    void ClearTacho(char outputs);
    void AnalogInit();
    void AnalogExit();
    void UARTInit();
    void UARTExit();
    void I2CInit();
    void I2CExit();
    void SetSensorTouch(char port);
    void SetSensorGyro(char port);
    void SetSensorUS(char port);
    void SetSensorColour(char port);
    void SetSensorNXTTouch(char port);
    void SetSensorNXTUS(char port);
    void SetSensorNXTLight(char port);
    void UIInit();
    void UIExit();
    char UpButtonState();
    char EnterButtonState();
    char DownButtonState();
    char RightButtonState();
    char LeftButtonState();
    char BackButtonState();
    void *ExitChecker(void *threadmain);
    void EV3Init();
    void EV3Exit();
#ifdef	__cplusplus
}
#endif
//</editor-fold>
#endif	/* EV3LIB_H */

