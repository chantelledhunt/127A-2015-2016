/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * Copyright (c) 2011-2014, Purdue University ACM SIG BOTS.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Purdue University ACM SIG BOTS nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL PURDUE UNIVERSITY ACM SIG BOTS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Purdue Robotics OS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"

/*
 * Runs the user operator control code. This function will be started in its own task with the
 * default priority and stack size whenever the robot is enabled via the Field Management System
 * or the VEX Competition Switch in the operator control mode. If the robot is disabled or
 * communications is lost, the operator control task will be stopped by the kernel. Re-enabling
 * the robot will restart the task, not resume it from where it left off.
 *
 * If no VEX Competition Switch or Field Management system is plugged in, the VEX Cortex will
 * run the operator control task. Be warned that this will also occur if the VEX Cortex is
 * tethered directly to a computer via the USB A to A cable without any VEX Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is available and
 * the scheduler is operational. However, proper use of delay() or taskDelayUntil() is highly
 * recommended to give other tasks (including system tasks such as updating LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop, even if empty.
 */

//void opFlywheel() {
//
//	if (joystickGetDigital(1, 6, JOY_DOWN)){
//		motorSet(7, 30);
//		motorSet(8, -30);
//
//		delay(200);
//
//		motorSet(7, 60);
//		motorSet(8, -60);
//
//		delay(200);
//
//		motorSet(7, 90);
//		motorSet(8, -90);
//
//		delay(200);
//
//		motorSet(7, 127);
//		motorSet(8, -127);
//
//		delay(100);
//
//
//
//	}
//	else if (joystickGetDigital(1, 6, JOY_UP)){
//		motorSet(7, 90);
//		motorSet(8, -90);
//
//		delay(100);
//
//		motorSet(7, 60);
//		motorSet(8, -60);
//
//		delay(100);
//
//		motorSet(7, 30);
//		motorSet(8, -30);
//
//		delay(100);
//
//		motorSet(7, 0);
//		motorSet(8, 0);
//	}

// float speed = 0;
// int toggle = 0;
//
// if (joystickGetDigital(1, 6, JOY_DOWN)) {     ///Flywheel toggle
//	 toggle = abs(toggle - 1);
//	 	 while (joystickGetDigital(1, 6, JOY_DOWN)) {
//	 		 delay(20);
//	 	 }
// }
// if (toggle == 1 && speed < 127) {
//	 speed = speed + 1;
// } else if (toggle == 0 && speed > 0) {
//	 speed = speed - 1;
//
// }
//
// if (joystickGetDigital(1, 8, JOY_UP)) {            ///Flywheel speed up
// speed = speed + 10;
// while (joystickGetDigital(1, 8, JOY_UP)) {
// delay(20);
// }
// }
// if (joystickGetDigital(1, 8, JOY_DOWN)) {          ///Flywheel slow down
// speed = speed - 10;
// while (joystickGetDigital(1, 8, JOY_DOWN)) {
// delay(20);
// }
// }
// motorSet(7, -speed);
// motorSet(8, -speed);
//}
void opBase() {                            ///Left side base
	if (abs(joystickGetAnalog(1, 3)) > 15) {
		motorSet(1, joystickGetAnalog(1, 3));
		motorSet(2, joystickGetAnalog(1, 3));
		motorSet(3, joystickGetAnalog(1, 3));
	} else {
		motorSet(1, 0);
		motorSet(2, 0);
		motorSet(3, 0);
	}                                        ///Right side base
	if (abs(joystickGetAnalog(1, 2)) > 15) {
		motorSet(4, -joystickGetAnalog(1, 2));
		motorSet(8, -joystickGetAnalog(1, 2));
		motorSet(6, -joystickGetAnalog(1, 2));
	} else {
		motorSet(4, 0);
		motorSet(8, 0);
		motorSet(6, 0);
	}
}
void opIntake() {
	if (joystickGetDigital(1, 7, JOY_DOWN)) {     ///Down/outtake
		motorSet(9, 127);
	} else if (joystickGetDigital(1, 7, JOY_UP)) {  ///Up or intake
		motorSet(9, -127);
	} else {
		motorSet(9, 0);
	}

	if (joystickGetDigital(1, 5, JOY_DOWN)) {    ///Intake
		motorSet(10, 127);
	} else if (joystickGetDigital(1, 5, JOY_UP)) { ///Outtake
		motorSet(10, -127);
	} else {
		motorSet(10, 0);
	}
} //[pp[]]

float set = 3;
float currentSpeed = 0;
float vLast = 0;
float iLast = -0.01;
float i = 0;
float v = 0;
float speed = 0;
int manual = 0;
int counts = 0;
void opFlywheel() {

	int counts = encoderGet(leftflywheelquadencoder);
		encoderReset(leftflywheelquadencoder);
		delay(1000);
		currentSpeed = counts/360;

//	if (joystickGetDigital(1, 6, JOY_DOWN)) {
//		motorSet(7, 60);
//		motorSet(8, -60);
//
//		delay(500);
//
//		motorSet(7, 127);
//		motorSet(8, -127);
//	}
//
//	if (joystickGetDigital(1, 6, JOY_UP)) {
//		motorSet(7, 60);
//		motorSet(8, -60);
//
//		delay(500);
//
//		motorSet(7, 0);
//		motorSet(8, 0);
//	}
//
//	if (joystickGetDigital(1, 8, JOY_UP)) {
//		motorSet(7, 60);
//		motorSet(8, -60);
//	}
//
//	if(joystickGetDigital(1, 8, JOY_RIGHT)) {
//		motorSet(7, 100);
//		motorSet(8, -100);
//	}

if (joystickGetDigital(1, 8, JOY_LEFT)) {
	manual = abs(manual - 1);
	while(joystickGetDigital(1, 8, JOY_LEFT)) {
		delay(50);
		}

	}

//	if(joystickGetDigital(1, 6, JOY_DOWN)){ ///Gradual speed up
//		speed+=.3;
//	}
//
//	if(joystickGetDigital(1, 6, JOY_UP)){   ///Gradual slow  down
//		speed-=.3;
//
//
//	}
//	if(joystickGetDigital(1, 8, JOY_UP)){   ///Increase speed by 10
//		speed+=10;
//		while(joystickGetDigital(1, 8, JOY_UP)){
//				delay(50);
//			}
//	}
//	if(joystickGetDigital(1, 8, JOY_DOWN)){   ///Decrease speed by 10
//		speed-=10;
//		while(joystickGetDigital(1, 8, JOY_DOWN)){ ///Debounce
//			delay(50);
//		}
//	}
//	if(joystickGetDigital(1, 8, JOY_RIGHT)){   ///Set speed to 100
//		speed = 100;
//
//	}
//
//	if(joystickGetDigital(1, 8, JOY_LEFT)){     ///Set speed to 77
//		speed = 77;
//	}
//
//	if(joystickGetDigital(1, 7, JOY_LEFT)){     ///Increase speed by 5
//		speed+=5;
//		while(joystickGetDigital(1, 7, JOY_LEFT)){
//				delay(50);
//			}
//	}
//
//	if (joystickGetDigital(1, 7, JOY_RIGHT)){   ///Decrease speed by 5
//		speed-=5;
//		while(joystickGetDigital(1, 7, JOY_RIGHT)){
//				delay(50);
//			}
//	}
//
//	motorSet(5, -speed);            ///Corresponds speed to motor values
//	motorSet(7, speed);

	if (manual == 0){
		v =  set - currentSpeed;
		i += v;
	if((v<0) && (vLast<0)){
		i = (i + iLast)/2;
		}
		vLast = v;
		iLast = i;
		motorSet(9, 30 * i);
	}

}

void operatorControl() {

	while (1) {                ///Runs all functions
		opBase();
		opFlywheel();
		opIntake();

		delay(20);
	}
}
