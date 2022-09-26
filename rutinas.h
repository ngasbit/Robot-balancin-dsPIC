#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<libpic30.h>
#include "xc.h"

int it = 0;

void rutina1(void){
    for(it = 0; it <= 830; it++){
            CCW(it, it);
            delay_ms(5);
        }
        for(it = 830; it >= 0; it--){
            CCW(it, it);
            delay_ms(5);
        }
        for(it = 0; it <= 830; it++){
            CW(it, it);
            delay_ms(5);
        }
        for(it = 830; it >= 0; it--){
            CW(it, it);
            delay_ms(5);
        }
}