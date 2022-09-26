/*
 * File:   main.c
 * Author: ngasbit
 */
#include "config.h"
#include "MDriver.h"
#include "MPU6050.h"
#include "rutinas.h"
#include "QEI.h"
#include "PID.h"
// Position
float SP = 2;
float Kp = 350.0;
float Ki = 0.8;
float Kd = 10.0;
// Speed
float Kps = 100.0;
float Kis = 10.0;
float Kds = 0.0;
float offset = 1.5;

float Ts = 0.001;
float Max = 830;
float Min = -830;
float get = 0;

int PWM = 0;
float ppr = 48;
float relation = 20.4;
float getso = 0.0;

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void){
    get = computePID(angle());
    getso = computePIDs(get,offset,SpeedL());
    PWM = (int)(abs(get));
    if(PORTAbits.RA1 == 1 && PORTBbits.RB2 == 0 ){
        if(Out() > 0){
        CW(PWM , 0 );
        }
        if(Out() < 0){
        CCW(PWM , 0 );
        }
        
    }
    else if(PORTAbits.RA1 == 0 && PORTBbits.RB2 == 1 ){
        if(Out() > 0){
        CW(0 , PWM );
        }
        if(Out() < 0){
        CCW(0 , PWM );
        }   
    }
    else{
    if(Out() > 0){
        CW(PWM , PWM );
    }
    if(Out() < 0){
        CCW(PWM , PWM );
    }
    }
    _T1IF = 0;
}

int main(void) {
    PLL();
    initIO();
    init_PWM();
    init_motor(0,0);
    init_QEI();
    Motor_info(ppr,relation,Ts);
    init_I2C();
    MPU6050_Init();
    setPID(SP, Kp, Ki, Kd, Ts);
    setPIDs(Kps, Kis, Kds, Ts);
    set_VMAX_VMIN(Max, Min);
    _T1IP = 4;
    TMR1 = 0;
    PR1 = 157;
    T1CON = 0x8030;
    _T1IF = 0;
    _T1IE = 1;
    _IPL = 0;
    while(1){
    }
    return 0;
}
