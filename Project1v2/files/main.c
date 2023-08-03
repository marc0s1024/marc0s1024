// Binghamton University, EECE 287 Sophomore Design
// Final Project # 1
// Instructor: Meghana Jain

//Group: 24
//Student Names: Marcos Rodriguez, Nigel Webb

#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h> 
#include "zumo_drivers.h"

// define pins
#define LEFT_MOTOR_PWM 6
#define RIGHT_MOTOR_PWM 5
#define LEFT_MOTOR_DIR 2
#define RIGHT_MOTOR_DIR 1

// define one rotation of motor and coefficent to equalize motor speeds
#define ONE_ROTATION 909.7 // ~5.34375 inches
#define COEFF 1.025

// motor and button global variables
bool motor_dir_X = false;
bool motor_dir_Y = false;
bool button_a_is_pressed = false;
bool button_b_is_pressed = false;
bool button_c_is_pressed = false;
uint8_t speed = 0;
uint32_t one_inch = 0;
uint32_t turn_ratio = 0;

// oled globals
uint8_t hexCountX = 0;
uint8_t hexCountY = 0;
uint8_t hexDigitX1 = 0;
uint8_t hexDigitX2 = 0;
uint8_t hexDigitY1 = 0;
uint8_t hexDigitY2 = 0;

// quad globals
uint32_t rightCountX = 0;
uint32_t leftCountX = 0;
uint32_t rightCountY = 0;
uint32_t leftCountY = 0;
uint32_t rightCountTurn = 0;
uint32_t leftCountTurn = 0;
uint32_t distanceY = 0;
uint32_t distanceX = 0;
uint8_t countY = 0;
uint8_t countX = 0;

// configure motors
void config_motors() {
	//DDRB |= 0x66; is equivalent
	DDRB |= (1 << RIGHT_MOTOR_PWM);
	DDRB |= (1 << LEFT_MOTOR_PWM);
}

// set right motor speed
void right_motor_speed(uint8_t speed) {
	//OCR1A is the register to toggle right motor speed
	//coefficient normalizes motor speeds
	OCR1A = speed * COEFF;
}

// set left motor speed
void left_motor_speed(uint8_t speed) {
	//OCR1B is the register to toggle left motor speed
	OCR1B = speed;
}

// start motors and set to 60 speed
void start_motors(uint8_t speed) {
	speed = 60;
	right_motor_speed(speed);
	left_motor_speed(speed);
}

// set motors to forward direction
void forward_motors() {
	// set motors to forward
	PORTB &= ~(1 << LEFT_MOTOR_DIR);
	PORTB &= ~(1 << RIGHT_MOTOR_DIR);

	// set motor direction variable to true (forwards)
	motor_dir_X = true;
}

// set motor speeds to zero
void stop_motors() {
	// set functions controlling speed of both motors to zero
	right_motor_speed(0);
	left_motor_speed(0);
}

// toggle direction of both motors in X direction
void toggle_direction_X() {
	motor_dir_X = !motor_dir_X;
	if (motor_dir_X) {
		// motors reverse
		PORTB |= (1 << LEFT_MOTOR_DIR);
		PORTB |= (1 << RIGHT_MOTOR_DIR);

		// print 0 to oled when motors backward
		oled_set_cursor(3, 1);
		oled_put_hex(0);
	}
	else {
		// motors forward
		PORTB &= ~(1 << LEFT_MOTOR_DIR);
		PORTB &= ~(1 << RIGHT_MOTOR_DIR);

		// print 1 to oled when motors forward
		oled_set_cursor(3, 1);
		oled_put_hex(1);
	}
}

// toggle direction of both motors in Y direction
void toggle_direction_Y() {
	motor_dir_Y = !motor_dir_Y;
	if (motor_dir_Y) {
		// motors reverse
		PORTB |= (1 << LEFT_MOTOR_DIR);
		PORTB |= (1 << RIGHT_MOTOR_DIR);

		// print 0 to oled when motors backward
		oled_set_cursor(3, 4);
		oled_put_hex(0);
	}
	else {
		// motors forward
		PORTB &= ~(1 << LEFT_MOTOR_DIR);
		PORTB &= ~(1 << RIGHT_MOTOR_DIR);

		// print 1 to oled when motors forward
		oled_set_cursor(3, 4);
		oled_put_hex(1);
	}
}

// toggle motor direction in both X & Y directions
void toggle_direction() {
	toggle_direction_X();
	toggle_direction_Y();
}

// calculates distance for robot ot travel one inch forward or backwards
uint32_t oneInchUnit() {
	one_inch = ONE_ROTATION / 5.34375; // calulates distance to travel one inch 

	return one_inch;
}

// turn robot ninety degrees 
void turn_ninety(uint8_t speed) {
	// set motor directions for movement in each quadrant of grid
	// negative Y and X
	if (motor_dir_Y == true && motor_dir_X == true) {
		// right motor forward
		PORTB &= ~(1 << RIGHT_MOTOR_DIR);
		// left motor reverse
		PORTB |= (1 << LEFT_MOTOR_DIR);

		// complete turn
		turn_ratio = ONE_ROTATION * 0.710; // calulates distance to turn 90 degrees
	}
	// positive Y and X
	else if (motor_dir_Y == false && motor_dir_X == false) {
		// right motor reverse
		PORTB |= (1 << RIGHT_MOTOR_DIR);
		// left motor forward
		PORTB &= ~(1 << LEFT_MOTOR_DIR);

		// complete turn
		turn_ratio = ONE_ROTATION * 0.710; // calulates distance to turn 90 degrees
	}
	// negative Y and positive X
	else if (motor_dir_Y == true && motor_dir_X == false) {
		// right motor reverse
		PORTB |= (1 << RIGHT_MOTOR_DIR);
		// left motor forward
		PORTB &= ~(1 << LEFT_MOTOR_DIR);

		// complete turn
		turn_ratio = ONE_ROTATION * 0.750; // calulates distance to turn 90 degrees
	}
	// positive Y and negative X
	else if (motor_dir_Y == false && motor_dir_X == true) {
		// right motor forward
		PORTB &= ~(1 << RIGHT_MOTOR_DIR);
		// left motor reverse
		PORTB |= (1 << LEFT_MOTOR_DIR);

		// complete turn
		turn_ratio = ONE_ROTATION * 0.750; // calulates distance to turn 90 degrees
	}

	// turn on motors until robot turns 90 degrees
	rightCountTurn = get_right_quadrature_counter() - distanceY;
	leftCountTurn = get_left_quadrature_counter() - distanceY;

	while (rightCountTurn <= turn_ratio || leftCountTurn <= turn_ratio) {
		start_motors(speed);

		rightCountTurn = get_right_quadrature_counter() - distanceY;
		leftCountTurn = get_left_quadrature_counter() - distanceY;
	}
	stop_motors();
}

// calculate the distance to travel based on X coordinate input
void travelDistanceX(uint8_t speed, uint8_t hexCountX) {
	// reset motors to forward
	forward_motors();

	// set motors to correct direction
	if (motor_dir_X == true) {
		// motors forward
		PORTB &= ~(1 << LEFT_MOTOR_DIR);
		PORTB &= ~(1 << RIGHT_MOTOR_DIR);
	}
	else if (motor_dir_X == false) {
		// motors reverse
		PORTB |= (1 << LEFT_MOTOR_DIR);
		PORTB |= (1 << RIGHT_MOTOR_DIR);
	}
	
	countX = hexCountX;

	// calculate X distance
	if (countX > 0 && countX < 10) {
		distanceX = oneInchUnit() * (countX * 1.2) - 1; // -1 accounts for countX being one iteration more than displayed hex value
	}
	else if (countX >= 10 && countX < 20) {
		distanceX = oneInchUnit() * (countX * 1.05); // 1.05 accounts for decreased precision as distances increase
	}
	else if (countX >= 20 && countX < 32) {
		distanceX = oneInchUnit() * (countX * 1.10; // same as above 
	}
	else if (countX == 0) {
		distanceX = 0;
	}

	// turn on motors until calculated distance reached
	rightCountX = get_right_quadrature_counter() - distanceY;
	leftCountX = get_left_quadrature_counter() - distanceY;

	while (rightCountX <= distanceX || leftCountX <= distanceX) {
		start_motors(speed);

		rightCountX = get_right_quadrature_counter() - distanceY;
		leftCountX = get_left_quadrature_counter() - distanceY;
	}
	stop_motors();
}

// calculate the distance to travel based on Y coordinate input
void travelDistanceY(uint8_t speed, uint8_t hexCountY) {
	// set motors to correct direction
	if (motor_dir_Y == false) {
		// motors forward
		PORTB &= ~(1 << LEFT_MOTOR_DIR);
		PORTB &= ~(1 << RIGHT_MOTOR_DIR);
	}
	else if (motor_dir_Y == true) {
		// motors reverse
		PORTB |= (1 << LEFT_MOTOR_DIR);
		PORTB |= (1 << RIGHT_MOTOR_DIR);
	}
	
	countY = hexCountY;

	// calculate Y distance
	if (countY > 0 && countY < 10) {
		distanceY = oneInchUnit() * ((countY * 1.05)); // -1 accounts for countY being one iteration more than displayed hex value
	}
	else if (countY >= 10 && countY < 20) {
		distanceY = oneInchUnit() * (countY * 1.05); // 1.05 accounts for decreased precision as distances increase
	}
	else if (countY >= 20 && countY < 32) {
		distanceY = oneInchUnit() * (countY * 1.10); // same as above 
	}
	else if (countY == 0) {
		distanceY = 0;
	}

	// turn on motors until calculated distance reached
	rightCountY = get_right_quadrature_counter();
	leftCountY = get_left_quadrature_counter();

	while (rightCountY <= distanceY || leftCountY <= distanceY) {
		start_motors(speed);

		rightCountY = get_right_quadrature_counter();
		leftCountY = get_left_quadrature_counter();
	}
	stop_motors();
}


// driver for the OLED hex counter (X coordinate)
void countHexCoordX() {
	if (hexCountX < 16) {
		oled_put_hex(0); // first hex digit is 0 when less than decimal 16
		hexDigitX2 = hexCountX; // set second digit equal to hexCount
		oled_put_hex(hexDigitX2); // display hexCount in second hex digit
		hexCountX++;
	}
	else if (hexCountX >= 16) {
		hexDigitX1 = (hexCountX >> 4); //shift 4 bits to eliminate least significant hex char
		oled_put_hex(hexDigitX1); // displays most significant hexCount digit
		hexDigitX2 = hexCountX; // truncate hexCount to take least signficant digit
		oled_put_hex(hexDigitX2); // display least significant hexCount digit
		hexCountX++;
	}

	if (hexCountX == 31) {
		hexCountX = 0;
	}
}

// driver for the OLED hex counter (Y coordinate)
void countHexCoordY() {
	if (hexCountY < 16) {
		oled_put_hex(0); // first hex digit is 0 when less than decimal 16
		hexDigitY2 = hexCountY; // set second digit equal to hexCount
		oled_put_hex(hexDigitY2); // display hexCount in second hex digit
		hexCountY++;
	}
	else if (hexCountY >= 16) {
		hexDigitY1 = (hexCountY >> 4); //shift 4 bits to eliminate least significant hex char
		oled_put_hex(hexDigitY1); // displays most significant hexCount digit
		hexDigitY2 = hexCountY; // truncate hexCount to take least signficant digit
		oled_put_hex(hexDigitY2); // display least significant hexCount digit
		hexCountY++;
	}

	if (hexCountY == 31) {
		hexCountY = 0;
	}
}

int main(){
	configure_zumo();
	config_motors();
	stop_motors();

	// toggle direction forward (do not remove)
	toggle_direction();

	while (1) {

		// button a controls X coordinate input 
		if (button_a_is_down() && !button_a_is_pressed) {
			button_a_is_pressed = true;
		}
		else if (button_a_is_up() && button_a_is_pressed) {
			button_a_is_pressed = false;

			// set cursor 1,1
			oled_set_cursor(1, 1);
			countHexCoordX();

			// toggle motor direction when X coordinate is 0
			if (hexCountX == 1) {
				toggle_direction_X();
			}
		}

		// button b controls Y coordinate input 
		if (button_b_is_down() && !button_b_is_pressed) {
			button_b_is_pressed = true;
		}
		else if (button_b_is_up() && button_b_is_pressed) {
			button_b_is_pressed = false;

			// set cursor 1,4
			oled_set_cursor(1, 4);
			countHexCoordY();

			// toggle motor direction when Y coordinate is 0
			if (hexCountY == 1) {
				toggle_direction_Y();
			}
		}

		// button c will start run (enable motors)
		if (button_c_is_down() && !button_c_is_pressed) {
			button_c_is_pressed = true;
		}
		else if (button_c_is_up() && button_c_is_pressed) {
			button_c_is_pressed = false;

			speed = 60;
			if (speed > 60) {
				speed = 0; // reset speed to zero when 100%
			}

			// call functions that move robot
			travelDistanceY(speed, hexCountY);
			_delay_us(1000000);
			turn_ninety(speed);
			_delay_us(1000000);
			travelDistanceX(speed, hexCountX);
		}
	}

	return 0;
}