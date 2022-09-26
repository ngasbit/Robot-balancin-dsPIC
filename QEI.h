#include <xc.h>
#include <libpic30.h>// liberia para el delay
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

float ppr_ = 0;
float relation_ = 0;
float time_ = 0.001;
float speedr = 0.0;
float speedl = 0.0;
int pos_l = 0;
int pos_lp = 0;
int pos_r = 0;
int pos_rp = 0;
float fc = 0.0;


void init_QEI(void){
    RPINR14bits.QEA1R = 6;
    RPINR14bits.QEB1R = 7;
    POS1CNT = 0; //inicio
    QEI1CONbits.QEIM = 0b111; // resoluci?n 4x, modo de Reset por MAXCNT
    QEI1CONbits.QEISIDL = 0; // continuar en modo idle (0)
    QEI1CONbits.SWPAB = 0; // Phase-A y Phase-B intercambiados
    QEI1CONbits.PCDOUT = 1; // activado el pin de Position Counter Status
    QEI1CONbits.TQGATE = 0; // Timer gate apagado
    QEI1CONbits.TQCKPS = 0; // Prescaler 1:1
    QEI1CONbits.POSRES = 0; // Un pulso en INDEX no hace un reset
    QEI1CONbits.TQCS = 0; // Usamos clock interno para el timer
    QEI1CONbits.UPDN_SRC = 1; // Phase-B indica direcci?n
    MAX1CNT = 1000; //limite maximo
    DFLT1CONbits.QEOUT = 1;
    DFLT1CONbits.QECK = 0b111;
   
    
    RPINR16bits.QEA2R = 10; 
    RPINR16bits.QEB2R = 11;
    POS2CNT = 0; //inicio
    QEI2CONbits.QEIM = 0b111; // resoluci?n 4x, modo de Reset por MAXCNT
    QEI2CONbits.QEISIDL = 0; // continuar en modo idle (0)
    QEI2CONbits.SWPAB = 0; // Phase-A y Phase-B intercambiados
    QEI2CONbits.PCDOUT = 1; // activado el pin de Position Counter Status
    QEI2CONbits.TQGATE = 0; // Timer gate apagado
    QEI2CONbits.TQCKPS = 0; // Prescaler 1:1
    QEI2CONbits.POSRES = 0; // Un pulso en INDEX no hace un reset
    QEI2CONbits.TQCS = 0; // Usamos clock interno para el timer
    QEI2CONbits.UPDN_SRC = 1; // Phase-B indica direcci?n
    MAX2CNT = 1000; //limite maximo
    DFLT2CONbits.QEOUT = 1;
    DFLT2CONbits.QECK = 0b111;
}

void Motor_info(float g ,float h, float i){
    ppr_ = g;
    relation_ = h;
    time_ = i;
    fc = 6.283185/g*h;
}

float SpeedL(){
    pos_l = POS1CNT;
        speedl = (fc)*(pos_l - pos_lp )/time_;
        speedl = (fc)*(pos_l - pos_lp)/time_;
    pos_lp = pos_l; 
    return speedl;
}

float SpeedR(){
    pos_r = POS2CNT;
    speedr = (fc)*(pos_r - pos_rp)/time_;
    pos_rp = pos_r; 
    return speedr;
}

