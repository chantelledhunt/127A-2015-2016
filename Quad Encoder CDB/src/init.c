/** @file init.c
 * @brief File for initialization code
 *
 * This file should contain the user initialize() function and any functions related to it.
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
 * Runs pre-initialization code. This function will be started in kernel mode one time while the
 * VEX Cortex is starting up. As the scheduler is still paused, most API functions will fail.
 *
 * The purpose of this function is solely to set the default pin modes (pinMode()) and port
 * states (digitalWrite()) of limit switches, push buttons, and solenoids. It can also safely
 * configure a UART port (usartOpen()) but cannot set up an LCD (lcdInit()).
 */
void initializeIO() {

	pinMode(1, OUTPUT);
	digitalWrite(1, LOW);

}
/*
 * Runs user initialization code. This function will be started in its own task with the default
 * priority and stack size once when the robot is starting up. It is possible that the VEXnet
 * communication link may not be fully established at this time, so reading from the VEX
 * Joystick may fail.
 *
 * This function should initialize most sensors (gyro, encoders, ultrasonics), LCDs, global
 * variables, and IMEs.
 *
 * This function must exit relatively promptly, or the operatorControl() and autonomous() tasks
 * will not start. An autonomous mode selection menu like the pre_auton() in other environments
 * can be implemented in this task if desired.
 */

int page = 1;

void autonSelect() {
	while (!isEnabled()) {
		if (lcdReadButtons(uart1 ) == 1) {
			page -= 1;
			delay(200);
		}
		if (lcdReadButtons(uart1 ) == 4) {
			page += 1;
			delay(200);
		}

		if (page < 1) {
			page = 7;
		}
		if (page > 7) {
			page = 1;
		}

		if (page == 1) {
			lcdPrint(uart1, 1, "Battery is at %d", powerLevelMain());
		}

		if (page == 2) {
			lcdSetText(uart1, 1, "Normal Auton");
			if (lcdReadButtons(uart1 ) == 2) {
				autonMode = page;
			}
		}

		if (page == 3) {
			lcdSetText(uart1, 1, "Delay 5 Seconds");
			if (lcdReadButtons(uart1 ) == 2) {
				autonMode = page;
			}
		}

		if (page == 4) {
			lcdSetText(uart1, 1, "Red stack");
			if (lcdReadButtons(uart1 ) == 2) {
				autonMode = page;
			}
		}

		if (page == 5) {
			lcdSetText(uart1, 1, "No Autonomous");
			if (lcdReadButtons(uart1 ) == 2) {
				autonMode = page;
			}
		}

		if (page == 6) {
			lcdSetText(uart1, 1, "Programming Skills");
			if (lcdReadButtons(uart1 ) == 2) {
				autonMode = page;
			}
		}

		if (page == 7) {
			lcdSetText(uart1, 1, "Blue stack");
			if (lcdReadButtons(uart1 ) == 2) {
				autonMode = page;
			}
		}
		delay(20);
	}
}

void initialize() {
	lcdInit(uart1 );
	lcdClear(uart1 );

	autonSelect();
}
