/* 
 * File:   EV3lib.h
 * Author: Bram Mak
 *
 * Created on 27 mei 2015, 22:39
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
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

void SetMotorPolarity(char outputs, char polarity) //Does not work with OnFwdSync. To-Do: New polarity system
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

void WaitForMotor(char outputs) //Will not work, replace with opOutput_Test with pointer to bool
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
        motorcommand[0] = opOUTPUT_TIME_SYNC;
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
#ifdef OLDSENSORIMPL
int (*Sensor1Func)(char);
int (*Sensor2Func)(char);
int (*Sensor3Func)(char);
int (*Sensor4Func)(char);
#else
int (*SensorValFunc[SENSORPORTS])(char port);
#endif
char sensordatasize[SENSORPORTS] = {DATA_8, DATA_8, DATA_8, DATA_8};
int analogfile;
int uartfile;
int i2cfile;
ANALOG *panalog;
UART *puart;
IIC *pi2c;
char analoginit = false;
char uartinit = false;
char i2cinit = false;

/* //Using modified enum NXTCOLOR in bytecodes.h
enum COLOURS
{
    NONE,
    BLACK,
    BLUE,
    GREEN,
    YELLOW,
    RED,
    WHITE,
    BROWN
};
*/

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

int Pin1RawVal(char port)
{
    if(analoginit)
    {
        if(port <= S4)
        {
            return (panalog->Pin1[port][panalog->Actual[port]]);
        }
    }
    else SensorDevInitErr();
}

int Pin6RawVal(char port)
{
    if(analoginit)
    {
        if(port <= S4)
        {
            return (panalog->Pin6[port][panalog->Actual[port]]);
        }
    }
    else SensorDevInitErr();
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

int UARTRawVal(char port)
{
    if(uartinit)
    {
        if(port <= S4)
        {
            switch(sensordatasize[port])
            {
                case DATA_8:
                    return (int)*(char*)puart->Raw[port][puart->Actual[port]];
                    break;
                case DATA_16:
                    return (int)*(short*)puart->Raw[port][puart->Actual[port]];
                    break;
                case DATA_32:
                    return (int)*(int*)puart->Raw[port][puart->Actual[port]];
                    break;
            }
        }
    }
    else SensorDevInitErr();
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

int I2CRawVal(char port)
{
    if(i2cinit)
    {
        if(port <= S4)
        {
            switch(sensordatasize[port])
            {
                case DATA_8:
                    return (int)*(char*)pi2c->Raw[port][pi2c->Actual[port]];
                    break;
                case DATA_16:
                    return (int)*(short*)pi2c->Raw[port][pi2c->Actual[port]];
                    break;
                case DATA_32:
                    return (int)*(int*)pi2c->Raw[port][pi2c->Actual[port]];
                    break;
            }
        }
    }
    else SensorDevInitErr();
}

void AssignSensorFunc(char port, int (*sensorfunc)(char), char datasize)
{
    if(analoginit && uartinit && i2cinit)
    {
        #ifdef OLDSENSORIMPL
        switch(port)
        {
            case S1:
                Sensor1Func = sensorfunc;
                break;
            case S2:
                Sensor2Func = sensorfunc;
                break;
            case S3:
                Sensor3Func = sensorfunc;
                break;
            case S4:
                Sensor4Func = sensorfunc;
                break;
            default:
                printf("No valid port number! (%d)\n", port);
                return;
        }
        #else
        if(port <= S4) SensorValFunc[port] = sensorfunc;
        else printf("No valid port number! (%d)\n", port);
        #endif
        sensordatasize[port] = datasize;
    }
    else SensorDevInitErr();
}

void SetColourSensorMode(char port, char mode)
{
    DEVCON devcon;
    if(uartinit)
    {
        devcon.Connection[port] = CONN_INPUT_UART;
        devcon.Type[port] = 29; //Type no. of colour sensor
        devcon.Mode[port] = mode;
        ioctl(uartfile, UART_SET_CONN, &devcon);
        if(mode <= COLOUR) sensordatasize[port] = DATA_8;
        else sensordatasize[port] = DATA_16;
        //Compile Error -> No more! :D (forgot to include sys/ioctl.h #epicfail)
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

void NumOut(int x, int y, int num, char fieldwidth)
{
    char tmpstr[fieldwidth+1];
    char charswritten = 0;
    memset(tmpstr, ' ', fieldwidth);
    tmpstr[fieldwidth] = '\0';
    charswritten += snprintf(tmpstr, fieldwidth+1, "%d", num);
    if(charswritten < fieldwidth) tmpstr[charswritten] = ' ';
    dLcdDrawText(lcd.Lcd, FG_COLOR, x, y, NORMAL_FONT, (signed char *)tmpstr);
    if(lcdupdate == false) dLcdUpdate(&lcd);
}

void TextNumOut(int x, int y, char *str, int num, char fieldwidth)
{
    char tmpstr[fieldwidth + 1];
    char charswritten = 0;
    charswritten += snprintf(tmpstr, fieldwidth+1 , "%s%d", str, num);
    if(charswritten < fieldwidth)
    {
        memset(tmpstr + charswritten, ' ', fieldwidth - charswritten);
        tmpstr[fieldwidth] = '\0';
    }
    dLcdDrawText(lcd.Lcd, FG_COLOR, x, y, NORMAL_FONT, (signed char *)tmpstr);
    if(lcdupdate == false) dLcdUpdate(&lcd);
}
//</editor-fold>

//Time stuffs
//<editor-fold>
/*What names should I use? One for absolute time, the other relative to the last reset*/
struct timespec t0;

unsigned long CurrentTick()
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return ((time.tv_sec - t0.tv_sec) * 1000 + (time.tv_nsec - t0.tv_nsec) / 1000000);
}

/*
unsigned long CurrentTick()
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return ((time.tv_sec) * 1000 + (time.tv_nsec) / 1000000);
}
*/
//</editor-fold>

//Global stuffs
void *ExitChecker(void *threadmain)
{
    const struct timespec pollsleep = {0, 200000000L};
    LCDClear(lcd.Lcd);
    dLcdDrawText(lcd.Lcd, FG_COLOR, 0, ((LCD_HEIGHT - dLcdGetFontHeight(LARGE_FONT)) / 2), LARGE_FONT, (signed char*)"Running...");
    dLcdDrawText(lcd.Lcd, FG_COLOR, 0, (LCD_HEIGHT - dLcdGetFontHeight(TINY_FONT) - 1), TINY_FONT, (signed char*)"Using EV3-C by qwerty514");
    dLcdUpdate(&lcd);
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
    ClearTick();
}

void EV3Exit()
{
    PWMExit();
    TachoExit();
    AnalogExit();
    UARTExit();
    I2CExit();
    dLcdExit();
    UIExit();
    exit(0);
}
