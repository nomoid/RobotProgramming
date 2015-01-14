#pragma config(Hubs,  S4, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S1,     HTIRS2,         sensorI2CCustom)
#pragma config(Motor,  motorA,          clawFrontRight, tmotorNXT, openLoop)
#pragma config(Motor,  motorB,          clawSide,      tmotorNXT, openLoop)
#pragma config(Motor,  motorC,          clawFrontLeft, tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S4_C1_1,     rightWheel,    tmotorTetrix, openLoop, driveRight)
#pragma config(Motor,  mtr_S4_C1_2,     leftWheel,     tmotorTetrix, openLoop, driveLeft)
#pragma config(Motor,  mtr_S4_C2_1,     pulley1,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S4_C2_2,     pulley2,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S4_C3_1,     collector1,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S4_C3_2,     collector2,    tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S4_C4_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S4_C4_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S4_C4_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S4_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S4_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S4_C4_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
* 	Author: 	Markus Feng & Alex Guo
*			Team: 	DA Roboticss
*	 Project: 	Cascade Effect 2014
*  Program: 	Robot Controllers
*/

#include "hitechnic-irseeker-v2.h"
#include "JoystickDriver.c"

const int minSpeed = 5;
const int maxSpeed = 100;

const int wheelDist = 32.5;

const int irRegion = 5;

const float wheelCircum = 31.415926;

#include "JoystickDriver.c"


float absSqrt(float input){
	if(input < 0){
		return -sqrt(-input);
	}
	else{
		return sqrt(input);
	}
}

/*
	* 0: false
	* 1: true
	* -1: error
*/
int irFound(int value){
	int _dirAC = HTIRS2readACDir(HTIRS2);
	if (_dirAC < 0){
		return -1;
	}
	if(_dirAC == value){
		return 1;
	}
	else{
		return 0;
	}
}

void travelDistance(int leftSpeed, int rightSpeed, float distance, int mode){
	nMotorEncoder[leftWheel] = 0;
	nMotorEncoder[rightWheel] = 0;

	motor[leftWheel] = -leftSpeed;
	motor[rightWheel] = rightSpeed;


	while(/*(mode != 2 || irFound(irRegion) != 1) && */-1*nMotorEncoder[leftWheel] < distance*360/wheelCircum
		&& nMotorEncoder[rightWheel] < distance*360/wheelCircum){
		nxtDisplayTextLine(0, "%d", motor[leftWheel]);
		nxtDisplayTextLine(1, "%d", motor[rightWheel]);
		//nxtDisplayTextLine(0, "%d", nMotorEncoder[leftWheel]);
		//nxtDisplayTextLine(1, "%d", nMotorEncoder[rightWheel]);
		//nxtDisplayTextLine(2, "%d", distance);
		//nxtDisplayTextLine(3, "%d", distance*360/wheelCircum);
	}
}

void turn(float degrees, int speed, bool direction){
	//true = left, false = right
	float dist = wheelDist*3.14*degrees/180;

	if(direction){
		travelDistance(0, speed, dist, false);
	}else{
		travelDistance(speed, 0, dist, false);
	}
}


int irRead(){
  return HTIRS2readACDir(HTIRS2);
}



task main() {
	/*while(true){
		int j = irRead();
		//writeDebugStreamLine("Int i is    : %d", j);
		nxtDisplayTextLine(0, "%d", j);
	}
	*/
	short basej1_y2 = joystick.joy1_y2;
	short basej1_y1 = joystick.joy1_y1;

	/*
		* 0: not turning / arc
		* 1: turning
		* 2: until infrared
		* 3: distance sensor
	*/
	int encoder = 0;
	int initSpeed = 100;
	double arc = 0.6623;

	int data0[] = {100,360,500,90,15,90,10,90,100}; //distance/values of until
	int data1[] = {initSpeed, 				0, 		 initSpeed, initSpeed, initSpeed, initSpeed, initSpeed,         0, initSpeed}; //left speed
	int data2[] = {initSpeed, initSpeed, initSpeed*arc,         0, initSpeed,         0, initSpeed, initSpeed, initSpeed}; // right speed
	int data3[] = {0, 1, 2, 1, 3, 1, 0, 1, 0}; // mode, see thing on top



	for(int i = 0; i <=3; i++){

		//Start Debug
		getJoystickSettings(joystick);
		if(joy1Btn(8) == 1){
			 break;
		}
		//End Debug

		if(data3[i] == 0){
			travelDistance(data1[i], data2[i], data0[i], 0);
		}
		else if(data3[i] == 1){
			if(data1[i] == 0 && data2[i] != 0){
				turn(data0[i], data2[i], true);
			}
			else if(data1[i] !=0 && data2[i] == 0){
				turn(data0[i], data1[i], false);
			}
			else{
				//print("Error in data[][]...");
			}
		}
		else if(data3[i] == 2){
			travelDistance(data1[i], 8, data0[i], 2);
		}
		else if(data3[i] == 3){
			travelDistance(data1[i], data2[i], data0[i], 3);
		}
	}
	nxtDisplayTextLine(0, "%d", 1);
}
