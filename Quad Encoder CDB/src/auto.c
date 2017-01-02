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

bool buttonPressed = 0;

void drive(int left, int right) {
	motorSet(5, -left);
	motorSet(6, -right);
}

void lift(int motorControlValue) {
	motorSet(1, motorControlValue);
	motorSet(2, -motorControlValue);
	motorSet(4, -motorControlValue);
	motorSet(7, -motorControlValue);
	motorSet(8, -motorControlValue);
	motorSet(9, -motorControlValue);
}

void autonomous() {
	lift(127);
	delay(270);
	lift(0);
	drive(127, 127);
	delay(2500);
	drive(0,0);
	delay(100);
	drive(-127, -127);
	delay(200);
	drive(80,-80); //Turn
	delay(1100);
	drive(0, 0);
	lift(-70); //Lift descends
	delay(200);
	drive(127, 127);
	delay(400);
	drive(-127, -127); //Fold out fork
	delay(500);
	drive(127, 1270);
	lift(0);
	delay(1100);
	lift(127); //Lift star
	delay(600);
	lift(10);
	drive(-127, -127);
	delay(1400);
	lift(127);
	delay(200);
	drive(0,0);
	delay(300);
	lift(0);
//	lift(127);
//	delay(300);
//	lift(0);
//	delay(50);
//	while(digitalRead(1)==1){
//		lift(-40);
//	}
//	lift(0);
//	delay(500);
//	drive(127, 127);
//	delay(500);
//	drive(-127,-127);
//	delay(200);
//	drive(0, 80); //Turn to align with cube
//	delay(180);
//	drive(70, 70);
//	delay(2000);
//	drive(0,0);
//	delay(500);
//	lift(80);
//	delay(350); //Lift cube
//	lift(60);
//	delay(50);
//	lift(10);
//	delay(1500);
//	drive(-50, 50);
//	delay(1200); //Turn to throw
//	drive(0,0);
//	delay(200);
//	drive(-127, -127);
//	delay(300);
//	lift(127); //Throw
//	delay(400);
//	lift(0);
//	delay(100);
//	lift(-60); //Reset
//	delay(300);
//	drive(127, 127);
//	delay(800);
//	drive(0,0);
//	lift(0);
//	delay(600);
//	drive(127, 127);
//	delay(300);
//	drive(0,0);
//	lift(127); //Pick up stars
//	delay(500);
//	lift(10);
//	drive(-127, -127);
//	delay(1000);
//	lift(127);
//	delay(700);
//	drive(0,0);
//	lift(0);
//	delay(1000);
}

