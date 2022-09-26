#include <xc.h>
#include <libpic30.h>// liberia para el delay
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

////////////////////////////////////////////////////////////////////////////////
//Parametros para posicion

float SP_ = 0.0;
float In_ = 0.0;
float Kp_ = 0.0;
float Kd_ = 0.0;
float Ki_ = 0.0;

float error_ = 0.0;
float sum_error_ = 0.0;
float diff_error_ = 0.0;
float error_p = 0.0;

float p_term_ = 0.0;
float i_term_ = 0.0;
float d_term_ = 0.0;

float Out_ = 0.0;

////////////////////////////////////////////////////////////////////////////////
//Parametros para velocidad

float Kps_ = 0.0;
float Kds_ = 0.0;
float Kis_ = 0.0;

float errors_ = 0.0;
float sum_errors_ = 0.0;
float diff_errors_ = 0.0;
float error_ps = 0.0;

float p_terms_ = 0.0;
float i_terms_ = 0.0;
float d_terms_ = 0.0;

float Outs_ = 0.0;
//////////////////////////////////////////////////////////////


float Ts_ = 0.0;

int Max_ = 0;
int Min_ = 0;

int PWM_ = 0;
float PWM_esc_ = 0;

void setPID(float a, float b, float c, float d, float e){
    SP_ = a;
    Kp_ = b;
    Kd_ = c;
    Ki_ = d;
    Ts_ = e;
}

void setPIDs(float k, float l, float m, float n){
    Kps_ = k;
    Kds_ = l;
    Kis_ = m;
    Ts_ = n;
}

void set_VMAX_VMIN(float g, float h){
    Max_ = g;
    Min_ = h;  
}


float computePID(float f){
    error_ = SP_ - f; 
    sum_error_ += error_;
    diff_error_ = error_ - error_p;
    p_term_ = Kp_ * error_;
    i_term_ = Ki_ * sum_error_ * Ts_;
    d_term_ = Kd_ * diff_error_ / Ts_;
    Out_ = p_term_+ i_term_ + d_term_;
    error_p = error_;
    if (Out_ >= Max_){
        Out_ = Max_;
    }
    else if (Out_ <= Min_){
        Out_ = Min_;
    }
    return Out_;
}

float computePIDs(float p , float q, float j){
    errors_ = (p*9.36/830) + q - j; 
    sum_errors_ += errors_;
    diff_errors_ = errors_ - error_ps;
    p_terms_ = Kps_ * errors_;
    i_terms_ = Kis_ * sum_errors_ * Ts_;
    d_terms_ = Kds_ * diff_errors_ / Ts_;
    Outs_ = p_terms_+ i_terms_ + d_terms_;
    error_ps = errors_;
    if (Outs_ >= Max_){
        Outs_ = Max_;
    }
    return Outs_;
}


float Out(){
    return Out_;
}