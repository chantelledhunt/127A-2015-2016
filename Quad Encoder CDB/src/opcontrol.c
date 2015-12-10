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
}

float lastLeft;
float velocityLeft;
float deltaEncoderLeft;
float lastRight;
float velocityRight;
float deltaEncoderRight;
void calcVelocity(void*ignore) {
	while (1) {
		deltaEncoderLeft = encoderGet(leftflywheelquadencoder) - lastLeft;
		lastLeft = encoderGet(leftflywheelquadencoder);
		velocityLeft = deltaEncoderLeft / 72 * 35 / 3; //Converts quad encoder measure to
		//rotations per second on the flywheel itself
		deltaEncoderRight = encoderGet(rightflywheelquadencoder) - lastRight;
		lastRight = encoderGet(rightflywheelquadencoder);
		velocityRight = deltaEncoderRight / 72 * 35 / 3; //Converts quad encoder measure to
		//rotations per second on the flywheel itself
		delay(200);
	}
}

int manual = 0;
float speedLeft, speedLeftAtZero;
float speedRight, speedRightAtZero;
float errorLeft, lastErrorLeft;
float errorRight, lastErrorRight;
float targetLeft = 20;
float targetRight = 20;
bool leftFirstCross = 1;
bool rightFirstCross = 1;

void opFlywheel() {
	while (1) {
		if (joystickGetDigital(1, 8, JOY_RIGHT)) {
			manual = abs(manual - 1);
			while (joystickGetDigital(1, 8, JOY_LEFT)) {
				delay(50);
			}
		}

		if(joystickGetDigital(1, 6, JOY_DOWN)){
			targetLeft = 15;
			targetRight = 15;
		}

		if (manual == 0) {
			errorLeft = targetLeft - velocityLeft;
			errorRight = targetRight - velocityRight;  //Calculates proper speed

			speedLeft = speedLeft + (errorLeft * 0.01);
			speedRight = speedRight + (errorRight * 0.01); //Calculates proper speed

			if (speedLeft > 127) {
				speedLeft = 127;  //Failsafe
			}
			if (speedRight> 127) {
				speedRight = 127;  //Failsafe
						}

			if (speedLeft < 0) {
				speedLeft = 0;    //Failsafe
			}

			if (speedRight < 0) {
				speedRight = 0;    //Failsafe
			}



			if (errorLeft * lastErrorLeft < 0) { //If error crosses 0
				if (leftFirstCross == 1) {
				speedLeft = 40;                  //Fiddle factor variable

				leftFirstCross = 0;
				}

				else {
					speedLeft = 0.5 * (speedLeft + speedLeftAtZero);
				}
				speedLeftAtZero = speedLeft;

			}

			if(manual == 1){
			if(joystickGetDigital(1, 8, JOY_LEFT)){
			speedLeft = 67;
			speedRight = 67;

				}

			if(joystickGetDigital(1, 7, JOY_LEFT)){
			speedLeft += 3, speedLeft +=3;
				}

			if(joystickGetDigital(1, 7, JOY_RIGHT)){
			speedLeft -=3, speedRight -=3;
			}

			}



			if (errorRight * lastErrorRight < 0) { //If error crosses 0
				if (rightFirstCross == 1) {
				speedRight = 40;                  //Fiddle factor variable

				rightFirstCross = 0;
				}

				else {
					speedRight = 0.5 * (speedRight + speedRightAtZero);
				}
				speedRightAtZero = speedRight;

			}


			lastErrorRight = errorRight;
			lastErrorLeft = errorLeft;

		}
		motorSet(5, -speedRight);   //Right
		motorSet(7, speedLeft);    //Left
		delay(20);
	}
}
void operatorControl() {
	taskCreate(calcVelocity, TASK_DEFAULT_STACK_SIZE, NULL,
			TASK_PRIORITY_DEFAULT + 1);
	taskCreate(opFlywheel, TASK_DEFAULT_STACK_SIZE, NULL,
			TASK_PRIORITY_DEFAULT);
	while (1) {                ///Runs all functions
		opBase();
		opIntake();
		printf("%f; %f; \n", velocityLeft, speedLeft);
		delay(200);
	}
}
