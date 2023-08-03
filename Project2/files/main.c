// Binghamton University, EECE 287 Sophomore Design
// Final Project # 2
// Instructor: Meghana Jain

//Group: 
//Student Names: 

#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h> 
#include "zumo_drivers.h"

// define pins
#define FRONT_PROX_SENSOR 1
#define RIGHT_PROX_SENSOR 4
#define LEFT_PROX_SENSOR 5
#define LED_PMW 6
#define LED_SELECT 6
#define IR_LED_CONTROL 7

// globals
uint32_t remote_hex_value = 0;


void configProxSensors() {
	DDRF |= (1 << FRONT_PROX_SENSOR);
	DDRD |= (1 << RIGHT_PROX_SENSOR);
	DDRF |= (1 << LEFT_PROX_SENSOR);
	DDRC |= (1 << LED_PMW);
	DDRF |= (1 << LED_SELECT);

}

// driver for the OLED hex display
void displayIRHexValue() {
	remote_hex_value = wait_for_remote_command();

	
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
	

	while(1){


	}
	
	return 0;
}
