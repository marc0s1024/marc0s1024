//EECE 287, Spring 2023 Project 2
//Instructor: Meg Jain
// Group # - 24 
//Team Members - Marcos Rodriguez, Nigel Webb

#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h> 
#include "zumo_drivers.h"

// define pins
#define RIGHT_SENSOR_PIN 4 //PD4
#define LEFT_SENSOR_PIN 5
#define FRONT_SENSOR_PIN 1
#define LED_PWM_PIN 6
#define LED_SELECT_PIN 6
#define LEFT_MOTOR_PWM 6
#define RIGHT_MOTOR_PWM 5
#define LEFT_MOTOR_DIR 2
#define RIGHT_MOTOR_DIR 1

// define one rotation of motor and coefficent to equalize motor speeds
#define ONE_ROTATION 909.7 // ~5.34375 inches
#define COEFF 1.0165

// define hex value for each button used on IR remote
#define ONE 0xFE01FA11
#define TWO 0xFE01FB10
#define THREE 0xFE01FA91
#define FOUR 0xFE01FA51
#define FIVE 0xFE01FB50
#define SIX 0xFE01FAD1
#define SEVEN 0xFE01FA31
#define EIGHT 0xFE01FB30
#define NINE 0xFE01FAB1
#define ENTER 0xFE01FB20
#define RIGHT 0xFE01FAA1
#define LEFT 0xFE01FA21

// globals for oled display
uint32_t remote_command_hex = 0;
uint32_t remote_command_bunkers = 0;
uint32_t remote_command_move = 0;
uint32_t remote_command_reverse = 0;
uint8_t rc_hex[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

// globals for IR remote input 1-9 used to input number of bunkers
uint8_t num_of_bunkers = 0;
uint8_t current_num_bunkers = 0;

// global for number of turns
uint8_t num_of_turns = 0;

// motor global variables
bool motor_dir = false;
uint8_t speed = 80;
uint32_t one_inch = 0;
uint32_t turn_ratio = 0;
bool direction_enable = true;
uint32_t right_count_turn = 0;
uint32_t left_count_turn = 0;
uint32_t spin_count_turn = 0;


/*
The following operates the proximity sensors by transmitting pulses on both 
the left and right LEDs at six different brightness levels. 
For each sensor, it generates two numbers: the number of brightness levels for the left LEDs that activated the sensor, 
and the number of brightness levels for the right LEDs that activated the sensor. 
A higher reading corresponds to more IR light getting reflected to the sensor, 
which is influenced by the size, reflectivity, proximity, and location of nearby objects. 
However, the presence of other sources of 38 kHz IR pulses (e.g. from another robot) can also affect the readings.
*/


uint16_t BrightnessLevels[] = {4, 15, 32, 55, 85, 120}; //think - duty cyles

//right sensor
void configure_right_proximity_sensor()
{
	DDRD &= ~(1<<RIGHT_SENSOR_PIN);
	PORTD |= (1<<RIGHT_SENSOR_PIN);
}


//higher return value indicates you are closer to an object
uint8_t read_right_proximity_sensor() 
{
	//turn off the line sensor IR LEDS to remove interference
	DDRB &= ~(1<<7);
	PORTB &= ~(1<<7);
	
	uint8_t right_sensor_detection_factor = 0;
	
	for (int i = 0; i < 6; i++){ //6 is the number of brightness levels in the array

		uint16_t brightness_duty_cycle = BrightnessLevels[i];
		
		start_IR_emitters(0, brightness_duty_cycle); //0 indicates to use right IR emitters
		_delay_us(420);
		
		if(!(PIND & (1<<RIGHT_SENSOR_PIN)))
			right_sensor_detection_factor++;
		
		stop_IR_emitters();
		_delay_us(550);
		
		start_IR_emitters(1, brightness_duty_cycle); //1 indicates to use left IR emitters
		_delay_us(420);
		
		if(!(PIND & (1<<RIGHT_SENSOR_PIN)))
			right_sensor_detection_factor++;
		stop_IR_emitters();
		_delay_us(550);
		
	}
	return right_sensor_detection_factor;
}


void configure_proximity_sensors()
{
	void configure_right_proximity_sensor();
	//configure others
	void configure_left_proximity_sensor();
	void configure_front_proximity_sensor();
}


//left sensor
void configure_left_proximity_sensor()
{
	//configure left proximity sensor
	DDRF &= ~(1 << LEFT_SENSOR_PIN);
	PORTF |= (1 << LEFT_SENSOR_PIN);
}

uint8_t read_left_proximity_sensor() 
{
	//turn off line sensor IR LEDS to remove interference
	DDRB &= ~(1<<7);
	PORTB &= ~(1<<7);
	
	//------------write code here: refer to read_right_proximity_sensor ---------
	uint8_t left_sensor_detection_factor = 0;

	for (int i = 0; i < 6; i++) { //6 is the number of brightness levels in the array

		uint16_t brightness_duty_cycle = BrightnessLevels[i];

		start_IR_emitters(0, brightness_duty_cycle); //0 indicates to use right IR emitters
		_delay_us(420);

		if (!(PINF & (1 << LEFT_SENSOR_PIN)))
			left_sensor_detection_factor++;

		stop_IR_emitters();
		_delay_us(550);

		start_IR_emitters(1, brightness_duty_cycle); //1 indicates to use left IR emitters
		_delay_us(420);

		if (!(PINF & (1 << LEFT_SENSOR_PIN)))
			left_sensor_detection_factor++;
		stop_IR_emitters();
		_delay_us(550);

	}
	return left_sensor_detection_factor;
}

//front sensor
void configure_front_proximity_sensor()
{
	//configure right proximity sensor
	DDRF &= ~(1 << FRONT_SENSOR_PIN);
	PORTF |= (1 << FRONT_SENSOR_PIN);
}

uint8_t read_front_proximity_sensor()
{
	//turn off line sensor IR LEDS to remove interference
	DDRB &= ~(1<<7);
	PORTB &= ~(1<<7);
	
	//------------write code here: refer to read_right_proximity_sensor ---------
	uint8_t front_sensor_detection_factor = 0;

	for (int i = 0; i < 6; i++) { //6 is the number of brightness levels in the array

		uint16_t brightness_duty_cycle = BrightnessLevels[i];

		start_IR_emitters(0, brightness_duty_cycle); //0 indicates to use right IR emitters
		_delay_us(420);

		if (!(PINF & (1 << FRONT_SENSOR_PIN)))
			front_sensor_detection_factor++;

		stop_IR_emitters();
		_delay_us(550);

		start_IR_emitters(1, brightness_duty_cycle); //1 indicates to use left IR emitters
		_delay_us(420);

		if (!(PINF & (1 << FRONT_SENSOR_PIN)))
			front_sensor_detection_factor++;
		stop_IR_emitters();
		_delay_us(550);

	}
	return front_sensor_detection_factor;
}

// motor functions //

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
	right_motor_speed(speed);
	left_motor_speed(speed);
}

// set motor speeds to zero
void stop_motors() {
	// set functions controlling speed of both motors to zero
	right_motor_speed(0);
	left_motor_speed(0);
}

// set motors to forward direction
void forward_motors() {
	// set motors to forward
	PORTB &= ~(1 << LEFT_MOTOR_DIR);
	PORTB &= ~(1 << RIGHT_MOTOR_DIR);

	// set motor direction variable to true (forwards)
	motor_dir = true;
}

void reverse_motors() {
	// set motors to reverse
	PORTB |= (1 << LEFT_MOTOR_DIR);
	PORTB |= (1 << RIGHT_MOTOR_DIR);

	// set motor direction variable to true (forwards)
	motor_dir = false;
}

// toggle direction of both motors
void toggle_direction() {
	motor_dir = !motor_dir;
	if (motor_dir) {
		// motors reverse
		PORTB |= (1 << LEFT_MOTOR_DIR);
		PORTB |= (1 << RIGHT_MOTOR_DIR);
	}
	else {
		// motors forward
		PORTB &= ~(1 << LEFT_MOTOR_DIR);
		PORTB &= ~(1 << RIGHT_MOTOR_DIR);
	}
}

// turn robot ninety degrees 
void turn_ninety_left(uint8_t speed) {
	// set turn speed
	speed = 60;

	// right motor forward
	PORTB &= ~(1 << RIGHT_MOTOR_DIR);
	// left motor reverse
	PORTB |= (1 << LEFT_MOTOR_DIR);

	// complete turn
	turn_ratio = ONE_ROTATION * 0.709025; // calulates distance to turn 90 degrees

	// turn on motors until robot turns 90 degrees
	for (left_count_turn = 0; left_count_turn < turn_ratio; left_count_turn += 55) {
		start_motors(speed);
		_delay_us(100000);
	}
	stop_motors();
}

void turn_ninety_right(uint8_t speed) {
	// set turn speed
	speed = 60;
	
	// right motor reverse
	PORTB |= (1 << RIGHT_MOTOR_DIR);
	// left motor forward
	PORTB &= ~(1 << LEFT_MOTOR_DIR);

	// complete turn
	turn_ratio = ONE_ROTATION * 0.709025; // calulates distance to turn 90 degrees

	// turn on motors until robot turns 90 degrees
	for (right_count_turn = 0; right_count_turn < turn_ratio; right_count_turn += 55) {
		start_motors(speed);
		_delay_us(100000);
	}
	stop_motors();
}

void turn_360(uint8_t speed) {
	// set turn speed
	speed = 150;

	// right motor reverse
	PORTB |= (1 << RIGHT_MOTOR_DIR);
	// left motor forward
	PORTB &= ~(1 << LEFT_MOTOR_DIR);

	// complete turn
	turn_ratio = ONE_ROTATION; // calulates distance to turn 360 degrees

	// turn on motors until robot turns 90 degrees
	for (spin_count_turn = 0; spin_count_turn < turn_ratio; spin_count_turn += 7.0) {
		start_motors(speed);
		_delay_us(10000);
	}
	stop_motors();
}

// gets hex values for each remote button input (CHECKPOINT FUNCTION)
void getRemoteInput() {
	remote_command_hex = wait_for_remote_command();
	uint8_t element_count = 0;
	oled_set_cursor(0, 0);

	if (remote_command_hex <= 0xFFFFFFFF && element_count < 8) {
		for (int j = 28; j >= 0; j = j - 4) {
			rc_hex[element_count] = (remote_command_hex >> j);
			oled_put_hex(rc_hex[element_count]);
			element_count++;
		}
	}
}

// convert remote hex values to corresponding printed button value
void inputNumBunkers() {
	remote_command_bunkers = wait_for_remote_command();

	// display numeric value 1-9 on OLED and enable set num_of_bunkers to IR input value
	switch (remote_command_bunkers) {
	case ONE: // defined at top
		num_of_bunkers = 1;
		oled_clear();
		oled_set_cursor(3, 4);
		oled_put_hex(1);
		break;
	case TWO:
		num_of_bunkers = 2;
		oled_clear();
		oled_set_cursor(3, 4);
		oled_put_hex(2);
		break;
	case THREE:
		num_of_bunkers = 3;
		oled_clear();
		oled_set_cursor(3, 4);
		oled_put_hex(3);
		break;
	case FOUR:
		num_of_bunkers = 4;
		oled_clear();
		oled_set_cursor(3, 4);
		oled_put_hex(4);
		break;
	case FIVE:
		num_of_bunkers = 5;
		oled_clear();
		oled_set_cursor(3, 4);
		oled_put_hex(5);
		break;
	case SIX:
		num_of_bunkers = 6;
		oled_clear();
		oled_set_cursor(3, 4);
		oled_put_hex(6);
		break;
	case SEVEN:
		num_of_bunkers = 7;
		oled_clear();
		oled_set_cursor(3, 4);
		oled_put_hex(7);
		break;
	case EIGHT:
		num_of_bunkers = 8;
		oled_clear();
		oled_set_cursor(3, 4);
		oled_put_hex(8);
		break;
	case NINE:
		num_of_bunkers = 9;
		oled_clear();
		oled_set_cursor(3, 4);
		oled_put_hex(9);
		break;
	}
}

bool isCompleted() {
	// if all bunkers found, return true
	if (current_num_bunkers == num_of_bunkers) {
		return true;
	}
	return false;
}


void objectDetector() {
	// display initial number of bunkers found
	oled_set_cursor(3, 8);
	oled_put_hex(current_num_bunkers);
	
	// front sensor object detection
	oled_set_cursor(0, 6);
	if (read_front_proximity_sensor() >= 0x0C) { // if object detected
		_delay_us(1200000);
		
		// put 1 to show object is found
		oled_put_hex(1);

		stop_motors();
		_delay_us(100000);

		// update current num of bunkers found
		oled_set_cursor(3, 8);
		oled_put_hex(++current_num_bunkers);

		// reverse when object found
		reverse_motors();
		start_motors(speed);

		_delay_us(100000);
		stop_motors();

		_delay_us(100000);
		
		// check if all bunkers found
		if (isCompleted()) {
			// if all bunkers found, do a spinny
			turn_360(speed);
			_delay_us(10);

			turn_360(speed);
			_delay_us(10);

			stop_motors();
		}

		remote_command_move = wait_for_remote_command(); 
		if (remote_command_move == RIGHT) { 	// if right button selected
			// turn right
			turn_ninety_right(speed);
			
			// reset remote_command_turn
			remote_command_move = 0;

			// increment num_of_turns
			num_of_turns++;
		}
		else if (remote_command_move == LEFT) { 	// if left button selected
			// turn left
			turn_ninety_left(speed);
			
			// reset remote_command_turn
			remote_command_move = 0;

			// increment num_of_turns
			num_of_turns++;	
		}
		else if (remote_command_move == ENTER) { 	// if enter button selected
			// reset position
			reverse_motors();
			start_motors(speed);
			_delay_us(1000000);

			stop_motors();

			// reset remote_command_turn
			remote_command_move = 0;
		}
		else { 	// if any other button input
			// do nothing
			_delay_us(10);
		}
	}
	else if (read_front_proximity_sensor() < 0x0C) { // if no object detected
		// put 0 to show no object is found
		oled_put_hex(0);

		forward_motors();
		_delay_us(10);
		start_motors(speed);
	}
}

void debug() {
	remote_command_move = wait_for_remote_command();
	if (remote_command_move == RIGHT) { 	// if right button selected
		// turn right
		turn_ninety_right(speed);

		// reset remote_command_turn
		remote_command_move = 0;

		// increment num_of_turns
		num_of_turns++;
	}
	else if (remote_command_move == LEFT) { 	// if left button selected
		// turn left
		turn_ninety_left(speed);

		// reset remote_command_turn
		remote_command_move = 0;

		// increment num_of_turns
		num_of_turns++;
	}
	else if (remote_command_move == ENTER) { 	// if enter button selected
		// reset position
		reverse_motors();
		start_motors(speed);
		_delay_us(1000000);

		stop_motors();

		// reset remote_command_turn
		remote_command_move = 0;
	}
	else if (remote_command_move == THREE) {
		// if all bunkers found, do a spinny
		turn_360(speed);
		_delay_us(10);

		turn_360(speed);
		_delay_us(10);

		stop_motors();
	}
	else { 	// if any other button input
		// do nothing
		_delay_us(10);
	}
}


int main(){
	
	configure_zumo();
	configure_proximity_sensors();

	config_motors();
	stop_motors();
	
	oled_clear();
	oled_set_cursor(0,0);

	// convert and display IR remote input value for number of bunkers ONCE in beginning of program
	inputNumBunkers();
	
	while(1){

		// detects proximity of bunker, stops and waits for IR input to turn right or left
		objectDetector();

		// if all bunkers found, break loop
		if (isCompleted()) {
			break;
		}
		
		// debug function
		//debug();

		// CHECKPOINT FUNCTION
		//getRemoteInput();

		// debug: display one_to_nine array values
		/*
		oled_set_cursor(4, 1);
		for (int i = 0; i < 9; i++) {
			oled_put_hex(one_to_nine[i]);
		}
		*/

	}
	
	return 0;
}
