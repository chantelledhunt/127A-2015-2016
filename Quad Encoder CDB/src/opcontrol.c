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
int isBraked = 0;

void opBase() {                            ///Left side base
	if (abs(joystickGetAnalog(1, 3)) > 15) {
		motorSet(6, -joystickGetAnalog(1, 3));
		motorSet(7, joystickGetAnalog(1, 3));
		motorSet(8, joystickGetAnalog(1, 3));
	} else {
		motorSet(6, 0);
		motorSet(7, 0);
		motorSet(8, 0);
	}                                        ///Right side base
	if (abs(joystickGetAnalog(1, 2)) > 15) {
		motorSet(3, -joystickGetAnalog(1, 2));
		motorSet(4, -joystickGetAnalog(1, 2));
		motorSet(5, joystickGetAnalog(1, 2));
	} else {
		motorSet(3, 0);
		motorSet(4, 0);
		motorSet(5, 0);
	}

	if (joystickGetDigital(1, 7, JOY_DOWN)) {  //Brake toggle
		isBraked = abs(isBraked - 1);
		delay(400);
	}
	if (isBraked == 1) {
		digitalWrite(2, LOW);
	} else if (isBraked == 0) {
		digitalWrite(2, HIGH);
	}
}

int topIntake = 0;
int bottomIntake = 0;
int isClosed = 0;

void opIntake() {
	if (joystickGetDigital(1, 5, JOY_UP)) {     //Sets both intakes to suck in
		topIntake = 127;
		bottomIntake = -127;
	} else if (joystickGetDigital(1, 5, JOY_DOWN)) { //Sets both intakes to spit out
		topIntake = -127;
		bottomIntake = 127;
	} else {
		topIntake = 0;
		bottomIntake = 0;
	}

	if (joystickGetDigital(1, 6, JOY_UP)) {      //Sets top intake to go up
		topIntake = 127;
	}

	if (joystickGetDigital(1, 6, JOY_DOWN)) {      //Sets top intake to go down
		topIntake = -127;
	}

	if (joystickGetDigital(1, 7, JOY_LEFT)) {      //Sets bottom intake to go up
		bottomIntake = -127;
	}

	if (joystickGetDigital(1, 7, JOY_RIGHT)) {
		bottomIntake = 127;
	}

	if (joystickGetDigital(2, 6, JOY_UP)) { //Partner joystick top intake control
		bottomIntake = -127;
	}
	if (joystickGetDigital(2, 6, JOY_DOWN)) {
		bottomIntake = 127;
	}
	motorSet(1, topIntake);
	motorSet(10, bottomIntake);
	lcdSetText(uart1, 1, "Above joystick");
	if (joystickGetDigital(1, 7, JOY_UP)) {   //Toggles top stopper
		isClosed ^= 1;
		if (isClosed) {
			lcdSetText(uart1, 1, "Opening");
			digitalWrite(1, HIGH);
		} else {
			lcdSetText(uart1, 1, "Closing");
			digitalWrite(1, LOW);
		}
		lcdSetText(uart1, 1, "Exiting");
		delay(400);
		lcdSetText(uart1, 1, "Debounce Done");
	}
	//lcdSetText(uart1, 1, "Out");

}

int speed = 0;
int longShot = 127;
int shortShot = 77;

void opFlywheel() {
	if (joystickGetDigital(1, 8, JOY_UP)) {    //Sets speed for long range shot
		speed = longShot;
	}

	if (joystickGetDigital(1, 8, JOY_DOWN)) {   //Sets speed for close shot
		speed = shortShot;
	}

	if (joystickGetDigital(1, 8, JOY_RIGHT)) {
		speed = speed - 5;
		delay(100);
	}

	if (joystickGetDigital(2, 7, JOY_DOWN)) {   //Partner controller commands
		shortShot = shortShot - 5;
		delay(100);
	}

	if (joystickGetDigital(2, 7, JOY_UP)) {
		shortShot = shortShot + 5;
		delay(100);
	}

	if (joystickGetDigital(2, 7, JOY_RIGHT)) {
		speed = 77;
	}
	if (joystickGetDigital(2, 7, JOY_LEFT)) {
		speed = 87;
		delay(500);
		speed = 77;
	}

	motorSet(2, -speed);
	motorSet(9, speed);

}

void operatorControl() {
	while (1) {
		lcdSetText(uart1, 1, "Base");///Runs all functions
		opBase();
		lcdSetText(uart1, 1, "Intake");
		opIntake();
		lcdSetText(uart1, 1, "Flywheel");
		opFlywheel();
		lcdSetText(uart1, 1, "Pause");
		delay(20);
		//lcdSetText(uart1, 1, "Hello, Chantelle.");
	}
}
