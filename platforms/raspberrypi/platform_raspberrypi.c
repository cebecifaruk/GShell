#include "../../GShell.h"
#include "bcm2835.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define X_EN    23
#define X_DIR   24
#define X_STEP  25

#define Y_EN    9
#define Y_DIR   10
#define Y_STEP  11

#define Z_EN    7
#define Z_DIR   8
#define Z_STEP  18

#define A_DIR   17
#define A_STEP  22
/*
#define us 1000
void Hardware::stepper(int fx, int fy, int fz, int fa, uint64_t T) {
    if(fx < 0) {fx*=-1; bcm2835_gpio_write(X_DIR,LOW);}
    else bcm2835_gpio_write(X_DIR,HIGH);

    if(fy < 0) {fy*=-1; bcm2835_gpio_write(Y_DIR,LOW);}
    else bcm2835_gpio_write(Y_DIR,HIGH);

    if(fz < 0) {fz*=-1; bcm2835_gpio_write(Z_DIR,LOW);}
    else bcm2835_gpio_write(Z_DIR,HIGH);

    if(fa < 0) {fa*=-1; bcm2835_gpio_write(A_DIR,LOW);}
    else bcm2835_gpio_write(A_DIR,HIGH);


    unsigned int tx,ty,tz,ta;
    unsigned int PULS_ON = 1;

    if(fx==0) tx=T;
    else tx=us/fx;
    if(fy==0) ty=T;
    else ty=us/fy;
    if(fz==0) tz=T;
    else tz=us/fz;
    if(fa==0) ta=T;
    else ta=us/fa;


    for (uint64_t i=0;i<=T;i++){

        if(i%tx == 0)bcm2835_gpio_set(X_STEP);
        if((i%tx) == PULS_ON)bcm2835_gpio_clr(X_STEP);
        if(i%ty == 0)bcm2835_gpio_set(Y_STEP);
        if((i%ty) == PULS_ON)bcm2835_gpio_clr(Y_STEP);
        if(i%ty == 0)bcm2835_gpio_set(Z_STEP);
        if((i%tz) == PULS_ON)bcm2835_gpio_clr(Z_STEP);
        if(i%ta == 0)bcm2835_gpio_set(A_STEP);
        if((i%ta) == PULS_ON)bcm2835_gpio_clr(A_STEP);

        bcm2835_delay(1);
    }
    return;
}
*/

void GEnableMotors() {
    bcm2835_gpio_set(X_EN);
    bcm2835_gpio_set(Y_EN);
    bcm2835_gpio_set(Z_EN);
    return;
}

void GDisableMotors(){
    bcm2835_gpio_clr(X_EN);
    bcm2835_gpio_clr(Y_EN);
    bcm2835_gpio_clr(Z_EN);
    return;
}
void GDelay (unsigned int ms) { usleep(1000 * ms);}

void GSetDir (Environment *env){

	if(env->motors[0].deltaSteps >= 0) bcm2835_gpio_clr(X_DIR);
	else {bcm2835_gpio_set(X_DIR);env->motors[0].deltaSteps*=-1;}
	if(env->motors[1].deltaSteps >= 0) bcm2835_gpio_clr(Y_DIR);
	else {bcm2835_gpio_set(Y_DIR);env->motors[1].deltaSteps*=-1;}
	if(env->motors[2].deltaSteps >= 0) bcm2835_gpio_clr(Z_DIR);
	else {bcm2835_gpio_set(Z_DIR);env->motors[2].deltaSteps*=-1;}
}
void GStepper(unsigned int step[AXIS_NUM])
{
	unsigned int k;
	// X Axis

	for(k=0;k<step[0];k++){
		bcm2835_gpio_set(X_STEP);
		bcm2835_delayMicroseconds (200);
		bcm2835_gpio_clr(X_STEP);
		bcm2835_delayMicroseconds (200);
	}

	for(k=0;k<step[1];k++){
		bcm2835_gpio_set(Y_STEP);
		bcm2835_delayMicroseconds (200);
		bcm2835_gpio_clr(Y_STEP);
		bcm2835_delayMicroseconds (200);
	}

	for(k=0;k<step[2];k++){
		bcm2835_gpio_set(Z_STEP);
		bcm2835_delayMicroseconds (200);
		bcm2835_gpio_clr(Z_STEP);
		bcm2835_delayMicroseconds (200);
	}

}
void init() {
    // Set the pin to be an output
    bcm2835_gpio_fsel(X_EN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(X_DIR, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(X_STEP, BCM2835_GPIO_FSEL_OUTP);

    bcm2835_gpio_fsel(Y_EN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(Y_DIR, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(Y_STEP, BCM2835_GPIO_FSEL_OUTP);

    bcm2835_gpio_fsel(Z_EN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(Z_DIR, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(Z_STEP, BCM2835_GPIO_FSEL_OUTP);

    bcm2835_gpio_fsel(A_DIR, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(A_STEP, BCM2835_GPIO_FSEL_OUTP);
}


int main(int argc, char *argv[]) {

    FILE *fp = fopen(argv[1], "r");
    //if( fp == NULL ) puts("ERROR: NO FILE"), exit(1);

    if (!bcm2835_init()) return 1;
    init();
    Environment env;
    GCreateEnv(&env);

    while (1) {
        if(fp==NULL) printf(">> "),gets(env.stdio);
        else if (fgets(env.stdio,150, fp) == NULL) break;
        GParse(&env);
        puts(env.stdio);
        GDisableMotors();
    }
    return 0;

}
