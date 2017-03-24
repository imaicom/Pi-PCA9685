// cc -o servo-allZero servo-allZero.c -lwiringPi -lm
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <math.h>

int resetPCA9685(int fd) {
	wiringPiI2CWriteReg8(fd,0,0);
}

int setPCA9685Freq(int fd , float freq) {
	float prescaleval;
	int prescale , oldmode , newmode;
	freq = 0.9 * freq;
	prescaleval = 25000000.0;
	prescaleval /= 4096.0;
	prescaleval /= freq;
	prescaleval -= 1.0;
	prescale = prescaleval + 0.5;
	oldmode = wiringPiI2CReadReg8(fd,0x00);
	newmode = (oldmode & 0x7F)|0x10;
	wiringPiI2CWriteReg8(fd , 0x00 , newmode);
	wiringPiI2CWriteReg8(fd , 0xFE , prescale);
	wiringPiI2CWriteReg8(fd , 0x00 , oldmode);
	sleep(0.005);
	wiringPiI2CWriteReg8(fd , 0x00 , oldmode | 0xA1);
}

int setPCA9685Duty(int fd , int channel , int on , int off) {
	int channelpos;
	channelpos = 0x6 + 4 * channel;
	wiringPiI2CWriteReg16(fd , channelpos   , on  & 0x0FFF);
	wiringPiI2CWriteReg16(fd , channelpos+2 , off & 0x0FFF);
}

int main() {
	int fd,i,j;
	
	fd = wiringPiI2CSetup(0x40);	// PCA9685
	resetPCA9685(fd);
	setPCA9685Freq(fd,50);	// 50Hz cycle

	while(1){
		for(i = 0; i < 16; i++) {
			setPCA9685Duty(fd , i , 0 , 143);	// PWM off timing 143(3.5%)~276(center)~410(100%)
		};
		delay(500);
		for(i = 0; i < 16; i++) {
			setPCA9685Duty(fd , i , 0 , 276);	// PWM off timing 143(3.5%)~276(center)~410(100%)
		};
		delay(500);
		for(i = 0; i < 16; i++) {
			setPCA9685Duty(fd , i , 0 , 410);	// PWM off timing 143(3.5%)~276(center)~410(100%)
		};
		delay(500);
		for(i = 0; i < 16; i++) {
			setPCA9685Duty(fd , i , 0 , 276);	// PWM off timing 143(3.5%)~276(center)~410(100%)
		};
		delay(500);		
	};
	return(0);
}
