/* 
 * File:   EV3lib.h
 * Author: Bram Mak
 *
 * Created on 27 mei 2015, 22:39
 */

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/mman.h>
#include <pthread.h>
#include <time.h>
#include "libEV3.h"
#include "d_lcd.h"

//Motor stuffs
//<editor-fold>
char motorcommand[10];
int pwmfile;
char pwminit = false;

void PWMDevInitErr()
{
    printf("The controller device for motors is not initialized.\n"
           "Don't forget to use \"EV3Init();\" and \"EV3Exit();\".\n");
}

void PWMInit()
{
    if((pwmfile = open(PWM_DEVICE_NAME, O_WRONLY)) == -1)
    {
        printf("Could not initialize motors!\n");
        close(pwmfile);
        exit(-1);
    }
    //Send
    motorcommand[0] = opPROGRAM_START;
    write(pwmfile, motorcommand, 2);
    pwminit = true;
}

void PWMExit()
{
    if(pwminit)
    {
        motorcommand[0] = opPROGRAM_STOP;
        write(pwmfile, motorcommand, 1);
        close(pwmfile);
        pwminit=false;
    }
}

void SetMotorPolarity(char outputs, char polarity)
{
    if(pwminit)
    {
        motorcommand[0] = opOUTPUT_POLARITY;
        motorcommand[1] = outputs;
        motorcommand[2] = polarity;
        write(pwmfile, motorcommand, 3);
    }
    else PWMDevInitErr();
}

void SetMotorType(char outputnumber, char type)
{
    if(pwminit)
    {
        if(outputnumber >= 0 && outputnumber <=3)
        {
            motorcommand[0] = opOUTPUT_SET_TYPE;
            motorcommand[1] = outputnumber;
            motorcommand[2] = type;
            write(pwmfile, motorcommand, 3);
        }
    }
    else PWMDevInitErr();
}

void ResetTacho(char outputs)
{
    if(pwminit)
    {
        motorcommand[0] = opOUTPUT_RESET;
        motorcommand[1] = outputs;
        write(pwmfile, motorcommand, 2);
    }
    else PWMDevInitErr();
}

void ReadTacho(char outputnumber, char *speed, int *count)
{
    if(pwminit)
    {
        if(outputnumber >= 0 && outputnumber <=3)
        {
            motorcommand[0] = opOUTPUT_READ;
            motorcommand[1] = outputnumber;
            memcpy(motorcommand+2, &speed, sizeof(speed));
            memcpy(motorcommand+6, &count, sizeof(count));
            write(pwmfile, motorcommand, 4);
        }
    }
    else PWMDevInitErr();
}

void WaitForMotor(char outputs)
{
    if(pwminit)
    {
        motorcommand[0] = opOUTPUT_READY;
        motorcommand[1] = outputs;
        write(pwmfile, motorcommand, 2);
    }
    else PWMDevInitErr();
}

void OnFwd(char outputs, char power)
{
    if(power > 100) power = 100;
    if(power < -100) power = -100;
    if(pwminit)
    {
        motorcommand[0] = opOUTPUT_POWER;
        motorcommand[1] = outputs;
        motorcommand[2] = power;
        write(pwmfile, motorcommand, 3);
        motorcommand[0] = opOUTPUT_START;
        motorcommand[1] = outputs;
        write(pwmfile, motorcommand, 2);
    }
    else PWMDevInitErr();
}

void OnFwdSpeed(char outputs, char speed)
{
    if(speed > 100) speed = 100;
    if(speed < -100) speed = -100;
    if(pwminit)
    {
        motorcommand[0] = opOUTPUT_SPEED;
        motorcommand[1] = outputs;
        motorcommand[2] = speed;
        write(pwmfile, motorcommand, 3);
        motorcommand[0] = opOUTPUT_START;
        motorcommand[1] = outputs;
        write(pwmfile, motorcommand, 2);
    }
    else PWMDevInitErr();
}

void OnFwdSync(char outputs, char speed, short turn)
{
    if(speed > 100) speed = 100;
    if(speed < -100) speed = -100;
    if(pwminit)
    {
        motorcommand[0] = opOUTPUT_STEP_SYNC;
        motorcommand[1] = outputs;
        motorcommand[2] = speed;
        memcpy(motorcommand+3, &turn, sizeof(turn));
        memset(motorcommand+5, 0, sizeof(int));
        motorcommand[9] = 0;
        write(pwmfile, motorcommand, 10);
    }
    else PWMDevInitErr();
}

void Off(char outputs)
{
    if(pwminit)
    {
        motorcommand[0] = opOUTPUT_STOP;
        motorcommand[1] = outputs;
        motorcommand[2] = BRAKE;
        write(pwmfile, motorcommand, 3);
    }
    else PWMDevInitErr();
}

void Float(char outputs)
{
    if(pwminit)
    {
        motorcommand[0] = opOUTPUT_STOP;
        motorcommand[1] = outputs;
        motorcommand[2] = FLOAT;
        write(pwmfile, motorcommand, 3);
    }
    else PWMDevInitErr();
}
//</editor-fold>

//Tacho stuffs
//<editor-fold>
MOTORDATA *ptacho;
int tachofile;
char tachoinit;

void TachoInit()
{
    if(tachoinit == false)
    {
        if((tachofile = open(MOTOR_DEVICE_NAME, O_RDWR | O_SYNC)) == -1)
        {
            printf("Failed to open tacho device.");
            exit(-1);
        }
        ptacho = (MOTORDATA*)mmap(0, sizeof(MOTORDATA)*vmOUTPUTS, PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, tachofile, 0);
        if(ptacho == MAP_FAILED)
        {
            printf("Failed to map tacho device\n");
            exit(-1);
        }
        tachoinit = true;
    }
}

void TachoExit()
{
    if(tachoinit)
    {
        int error;
        printf("Closing tacho device...\n");
        error = munmap(ptacho, sizeof(MOTORDATA)*vmOUTPUTS);
        if(error == -1) printf("Failed to unmap tacho device\n");
        else printf("Tacho device unmapped.\n");
        error = close(tachofile);
        if(error == -1) printf("Failed to close tacho device");
        else printf("tacho device closed.\n");
        tachoinit = false;
    }
}

void ClearTacho(char outputs)
{
    if(pwminit)
    {
        motorcommand[0] = opOUTPUT_CLR_COUNT;
        motorcommand[1] = outputs;
        write(pwmfile, motorcommand, 2);
    }
    else PWMDevInitErr();
}
//</editor-fold>

//Sensor stuffs
//<editor-fold>
void *psensor1;
void *psensor2;
void *psensor3;
void *psensor4;
int analogfile;
int uartfile;
int i2cfile;
ANALOG *panalog;
UART *puart;
IIC *pi2c;
char analoginit = false;
char uartinit = false;
char i2cinit = false;

void SensorDevInitErr()
{
    printf("The communication device for this sensor is not initialized.\n"
           "Don't forget to use \"EV3Init();\" and \"EV3Exit();\".\n");
}

void AnalogInit()
{
    if(analoginit == false)
    {
        if((analogfile = open(ANALOG_DEVICE_NAME, O_RDWR | O_SYNC)) == -1)
	{
		printf("Failed to open analog device\n");
		exit(-1); 
	}
	panalog  =  (ANALOG*)mmap(0, sizeof(ANALOG), PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, analogfile, 0);
	if (panalog == MAP_FAILED)
	{
		printf("Failed to map analog device\n");
		exit(-1);
	}
	printf("Analog device ready\n");
        analoginit = true;
    }
}

void AnalogExit()
{
    if(analoginit)
    {
        int error;
        printf("Closing analog device...\n");
        error = munmap(panalog, sizeof(ANALOG));
        if(error == -1) printf("Failed to unmap analog device\n");
        else printf("Analog device unmapped.\n");
        error = close(analogfile);
        if(error == -1) printf("Failed to close analog device");
        else printf("Analog device closed.\n");
        analoginit = false;
    }
}

void UARTInit()
{
    if(uartinit == false)
    {
        if((uartfile = open(UART_DEVICE_NAME, O_RDWR | O_SYNC)) == -1)
	{
		printf("Failed to open UART device\n");
		exit(-1); 
	}
	puart  =  (UART*)mmap(0, sizeof(UART), PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, uartfile, 0);
	if (puart == MAP_FAILED)
	{
		printf("Failed to map UART device\n");
		exit(-1);
	}
	printf("UART device ready\n");
        uartinit = true;
    }
}

void UARTExit()
{
        int error;
        printf("Closing UART device...\n");
        error = munmap(puart, sizeof(UART));
        if(error == -1) printf("Failed to unmap UART device\n");
        else printf("UART device unmapped.\n");
        error = close(uartfile);
        if(error == -1) printf("Failed to close UART device");
        else printf("UART device closed.\n");
        uartinit=false;
}

void I2CInit()
{
    if(i2cinit == false)
    {
        if((i2cfile = open(IIC_DEVICE_NAME, O_RDWR | O_SYNC)) == -1)
	{
		printf("Failed to open I2C device\n");
		exit(-1); 
	}
	pi2c = (IIC*)mmap(0, sizeof(IIC), PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, i2cfile, 0);
	if (pi2c == MAP_FAILED)
	{
		printf("Failed to map I2C device\n");
		exit(-1);
	}
	printf("I2C device ready\n");
        i2cinit = true;
    }
}

void I2CExit()
{
        int error;
        printf("Closing I2C device...\n");
        error = munmap(pi2c, sizeof(IIC));
        if(error == -1) printf("Failed to unmap I2C device\n");
        else printf("I2C device unmapped.\n");
        error = close(i2cfile);
        if(error == -1) printf("Failed to close I2C device");
        else printf("I2C device closed.\n");
        uartinit=false;
}

void SetSensorTouch(char port)
{
    if(analoginit)
    {
        switch(port)
        {
            case S1:
                psensor1 = (unsigned char*)&(panalog->Pin1[port][panalog->Actual[port]]);
                break;
            case S2:
                psensor2 = (unsigned char*)&(panalog->Pin1[port][panalog->Actual[port]]);
                break;
            case S3:
                psensor3 = (unsigned char*)&(panalog->Pin1[port][panalog->Actual[port]]);
                break;
            case S4:
                psensor4 = (unsigned char*)&(panalog->Pin1[port][panalog->Actual[port]]);
                break;
            default:
                printf("No valid port number! (%d)\n", port);
                break;
        }
    }
    else SensorDevInitErr();
}

void SetSensorGyro(char port)
{
    if(uartinit)
    {
        switch(port)
        {
            case S1:
                psensor1 = (unsigned char*)&(puart->Raw[port][puart->Actual[port]][0]);
                break;
            case S2:
                psensor2 = (unsigned char*)&(puart->Raw[port][puart->Actual[port]][0]);
                break;
            case S3:
                psensor3 = (unsigned char*)&(puart->Raw[port][puart->Actual[port]][0]);
                break;
            case S4:
                psensor4 = (unsigned char*)&(puart->Raw[port][puart->Actual[port]][0]);
                break;
            default:
                printf("No valid port number! (%d)\n", port);
                break;
        }
    }
    else SensorDevInitErr();
}

void SetSensorUS(char port)
{
    if(uartinit)
    {
        switch(port)
        {
            case S1:
                psensor1 = (unsigned char*)&(puart->Raw[port][puart->Actual[port]][0]);
                break;
            case S2:
                psensor2 = (unsigned char*)&(puart->Raw[port][puart->Actual[port]][0]);
                break;
            case S3:
                psensor3 = (unsigned char*)&(puart->Raw[port][puart->Actual[port]][0]);
                break;
            case S4:
                psensor4 = (unsigned char*)&(puart->Raw[port][puart->Actual[port]][0]);
                break;
            default:
                printf("No valid port number! (%d)\n", port);
                break;
        }
    }
    else SensorDevInitErr();
}

void SetSensorColour(char port)
{
    if(uartinit)
    {
        switch(port)
        {
            case S1:
                psensor1 = (unsigned char*)&(puart->Raw[port][puart->Actual[port]][0]);
                break;
            case S2:
                psensor2 = (unsigned char*)&(puart->Raw[port][puart->Actual[port]][0]);
                break;
            case S3:
                psensor3 = (unsigned char*)&(puart->Raw[port][puart->Actual[port]][0]);
                break;
            case S4:
                psensor4 = (unsigned char*)&(puart->Raw[port][puart->Actual[port]][0]);
                break;
            default:
                printf("No valid port number! (%d)\n", port);
                break;
        }
    }
    else SensorDevInitErr();
}

void SetSensorNXTTouch(char port)
{
    if(analoginit)
    {
        switch(port)
        {
            case S1:
                psensor1 = (unsigned char*)&(panalog->Pin1[port][panalog->Actual[port]]);
                break;
            case S2:
                psensor2 = (unsigned char*)&(panalog->Pin1[port][panalog->Actual[port]]);
                break;
            case S3:
                psensor3 = (unsigned char*)&(panalog->Pin1[port][panalog->Actual[port]]);
                break;
            case S4:
                psensor4 = (unsigned char*)&(panalog->Pin1[port][panalog->Actual[port]]);
                break;
            default:
                printf("No valid port number! (%d)\n", port);
                break;
        }
    }
    else SensorDevInitErr();
}

void SetSensorNXTUS(char port)
{
    if(i2cinit)
    {
        switch(port)
        {
            case S1:
                psensor1 = (unsigned char*)&(pi2c->Raw[port][puart->Actual[port]][0]);
                break;
            case S2:
                psensor2 = (unsigned char*)&(pi2c->Raw[port][puart->Actual[port]][0]);
                break;
            case S3:
                psensor3 = (unsigned char*)&(pi2c->Raw[port][puart->Actual[port]][0]);
                break;
            case S4:
                psensor4 = (unsigned char*)&(pi2c->Raw[port][puart->Actual[port]][0]);
                break;
            default:
                printf("No valid port number! (%d)\n", port);
                break;
        }
    }
    else SensorDevInitErr();
}

void SetSensorNXTLight(char port)                                               //NXT light sensor, EV3 only has coloursensor
{
    if(analoginit)
    {
        switch(port)
        {
            case S1:
                psensor1 = (unsigned char*)&(panalog->Pin1[port][panalog->Actual[port]]);
                break;
            case S2:
                psensor2 = (unsigned char*)&(panalog->Pin1[port][panalog->Actual[port]]);
                break;
            case S3:
                psensor3 = (unsigned char*)&(panalog->Pin1[port][panalog->Actual[port]]);
                break;
            case S4:
                psensor4 = (unsigned char*)&(panalog->Pin1[port][panalog->Actual[port]]);
                break;
            default:
                printf("No valid port number! (%d)\n", port);
                break;
        }
    }
    else SensorDevInitErr();
}
//</editor-fold>

//Button stuffs
//<editor-fold>
UI *pui;
int uifile;
char uiinit = false;
char ledcommand[2] = { 0, 0};

void ButtonDevInitErr()
{
    printf("The button device is not initialized.\n"
           "Don't forget to use \"EV3Init();\" and \"EV3Exit();\".\n");
}

void UIInit()
{
    if(uiinit == false)
    {
        if((uifile = open(UI_DEVICE_NAME, O_RDWR | O_SYNC)) == -1)
	{
		printf("Failed to open UI device\n");
		exit(-1); 
	}
	pui  =  (UI*)mmap(0, sizeof(UI), PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, uifile, 0);
	if (panalog == MAP_FAILED)
	{
		printf("Failed to map UI device\n");
		exit(-1);
	}
	printf("UI device ready\n");
        uiinit = true;
    }
}

void UIExit()
{
    if(uiinit)
    {
        int error;
        printf("Closing UI device...\n");
        error = munmap(pui, sizeof(UI));
        if(error == -1) printf("Failed to unmap UI device\n");
        else printf("UI device unmapped.\n");
        error = close(uifile);
        if(error == -1) printf("Failed to close UI device");
        else printf("UI device closed.\n");
        uiinit = false;
    }
}

char UpButtonState()
{
    if(uiinit) return pui->Pressed[0];
    else ButtonDevInitErr();
}

char EnterButtonState()
{
    if(uiinit) return pui->Pressed[1];
    else ButtonDevInitErr();
}

char DownButtonState()
{
    if(uiinit) return pui->Pressed[2];
    else ButtonDevInitErr();
}

char RightButtonState()
{
    if(uiinit) return pui->Pressed[3];
    else ButtonDevInitErr();
}

char LeftButtonState()
{
    if(uiinit) return pui->Pressed[4];
    else ButtonDevInitErr();
}

char BackButtonState()
{
    if(uiinit) return pui->Pressed[5];
    else ButtonDevInitErr();
}

void SetLedPattern(char pattern)
{
    if(uiinit)
    {
        ledcommand[0] = '0' + pattern;
        write(uifile, ledcommand, 2);
    }
    else ButtonDevInitErr();
}
//</editor-fold>

//LCD stuffs
//<editor-fold>
LCD lcd;
char lcdupdate = false;

void ClearScreen()
{
    LCDClear(lcd.Lcd);
    if(lcdupdate == false) dLcdUpdate(&lcd);
}

void UpdateScreen()
{
    dLcdUpdate(&lcd);
}

void TextOut(int x, int y, char *str)
{
    dLcdDrawText(lcd.Lcd, FG_COLOR, x, y, NORMAL_FONT,(signed char*)str);
    if(lcdupdate == false) dLcdUpdate(&lcd);
}

void NumOut(int x, int y, int num)
{
    char str[2];
    sprintf(str, "%d", num);
    dLcdDrawText(lcd.Lcd, FG_COLOR, x, y, NORMAL_FONT, (signed char *)str);
    if(lcdupdate == false) dLcdUpdate(&lcd);
}

void TextNumOut(int x, int y, char str[100], int num)
{
    sprintf(str+strlen(str), "%s%d", str, num);
    dLcdDrawText(lcd.Lcd, FG_COLOR, x, y, NORMAL_FONT, (signed char *)str);
    if(lcdupdate == false) dLcdUpdate(&lcd);
}
//</editor-fold>

//Global stuffs
void *ExitChecker(void *threadmain)
{
    const struct timespec pollsleep = {0, 500000000L};
    LCDClear(lcd.Lcd);
    dLcdDrawText(lcd.Lcd, FG_COLOR, 10, 20, NORMAL_FONT, "Running...   (press back to exit)");
    dLcdDrawText(lcd.Lcd, FG_COLOR, 10, (21 + dLcdGetFontHeight(NORMAL_FONT)), TINY_FONT, "Using EV3-C by qwerty514");
    while(true)
    {
        if(BackButtonState() == true)
        {
            pthread_cancel((pthread_t)threadmain);
            EV3Exit();
        }
        nanosleep(&pollsleep, NULL);
    }
}

void EV3Init()
{
    PWMInit();
    TachoInit();
    AnalogInit();
    UARTInit();
    I2CInit();
    dLcdInit(lcd.Lcd);
    UIInit();
    pthread_t threadbackground;
    pthread_t threadmain = pthread_self();
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    pthread_create(&threadbackground, NULL, ExitChecker, (void*)threadmain);
}

void EV3Exit()
{
    PWMExit();
    TachoInit();
    AnalogExit();
    UARTInit();
    I2CInit();
    dLcdExit();
    UIExit();
    exit(0);
}
