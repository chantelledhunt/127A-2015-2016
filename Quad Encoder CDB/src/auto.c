/** @file auto.c
 * @brief File for autonomous code
 *
 * This file should contain the user autonomous() function and any functions related to it.
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
 * Runs the user autonomous code. This function will be started in its own task with the default
 * priority and stack size whenever the robot is enabled via the Field Management System or the
 * VEX Competition Switch in the autonomous mode. If the robot is disabled or communications is
 * lost, the autonomous task will be stopped by the kernel. Re-enabling the robot will restart
 * the task, not re-start it from where it left off.
 *
 * Code running in the autonomous task cannot access information from the VEX Joystick. However,
 * the autonomous function can be invoked from another task if a VEX Competition Switch is not
 * available, and it can access joystick information if called in this way.
 *
 * The autonomous task may exit, unlike operatorControl() which should never exit. If it does
 * so, the robot will await a switch to another mode or disable/enable cycle.
 */

void drive(int left, int right) {
	motorSet(1, left);
	motorSet(2, left);
	motorSet(3, left);
	motorSet(4, right);
	motorSet(5, right);
	motorSet(6, right);
}

void intake(int motorControlValue) {
	motorSet(10, motorControlValue);
	motorSet(9, motorControlValue);
}

void flywheel(int motorControlValue) {
	motorSet(7, motorControlValue);
	motorSet(5  , -motorControlValue);
}

void driveEncoder(int target) {
	drive(100, 100);
	while (encoderGet(leftquadencoder) < target
			|| encoderGet(rightquadencoder) < target) {
		if (encoderGet(leftquadencoder) >= target) {
			motorSet(1, 0);
			motorSet(2, 0);
			motorSet(3, 0);
		}
		if (encoderGet(rightquadencoder) >= target) {
			motorSet(4, 0);
			motorSet(5, 0);
			motorSet(6, 0);
		}
		delay(20);
	}
}

void autonomous() {
//	driveEncoder(500);
//	flywheel(127); /*flyhweel speeds up*/
//	delay(500);
//	flywheel(127);
//	intake(127); /*intakes first ball*/
//	delay(200);
//	intake(0);
//	flywheel(127); /*shoots first ball*/
//	delay(600);
//	intake(127); /*intakes second ball*/
//	flywheel(127);
//	delay(200);
//	intake(0);
//	flywheel(127); /*shoots second ball*/
//	delay(600);
//	intake(127); /*intakes third ball*/
//	flywheel(127);
//	delay(200);
//	intake(0);
//	flywheel(127); /*shoots third ball*/
//	delay(600);
//	intake(127); /*intakes fourth ball*/
//	flywheel(127);
//	delay(200);
//	intake(0);
//	flywheel(127); /*shoots fourth ball*/
//	delay(600);
//	flywheel(100);/*flywheel slows down*/
//	delay(200);
//	flywheel(60);
//	delay(200);
//	flywheel(30);
//	delay(200);
//	flywheel(0);
//	delay(200);
flywheel(20);
delay(500);
flywheel(40);
delay(500);
flywheel(60);
delay(500);
flywheel(65);
delay(3500);

intake(127);

delay(1500);

intake(0);

delay(1500);

intake(127);

delay(1500);

intake(127);

delay(3500);

flywheel(20);
delay(200);
flywheel(40);
delay(200);
flywheel(60);
delay(200);
flywheel(75);
delay(1400);
}
