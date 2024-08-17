#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "keypad.h"
#include "lcd.h"
#include "DIO_Interface.h"

u8 count1 = 0, count2 = 0, operator = 0, input, flag_div = 0, flag_first = 0,
		flag_no_sec = 0, flag_next_num = 0, flag_op = 0;
u16 result = 0, first = 0, second = 0;

u16 operation(u16 first, u16 second, u8 operand) {
	u16 result = 0;
	switch (operand) {
	case '+':
		result = first + second;
		break;
	case '-':
		result = first - second;
		break;
	case '*':
		result = first * second;
		break;
	case '/':
		if (second == 0) {
			flag_div = 1;
		} else {
			result = first / second;
		}
		break;
	}
	return result;
}

void Interrupt_init(void) {
	DIO_SetPinDir(Port_D, Pin3, Input);
	DIO_SetPinValue(Port_D, Pin3, High);
	GICR |= (1 << 7);
	MCUCR |= (1 << 3);
	MCUCR &= ~(1 << 2);
	sei();
}

//interrupt to delete a character
ISR(INT1_vect) {

	//delete a char from the first number
	if (operator == 0) {
		first /= 10;
		count1--;
		LCD_moveCursor(0, count1);
		LCD_displayCharacter(' ');
		_delay_ms(40);
		LCD_moveCursor(0, count1);
	}
	//delete a char from the second number
	else if (count2 > 0 && second > 0) {
		second /= 10;
		count2--;
		LCD_moveCursor(1, count2);
		LCD_displayCharacter(' ');
		_delay_ms(40);
		LCD_moveCursor(1, count2);
	}
	else if (second == 0 && operator != 0) {
		flag_op = 1;
		LCD_moveCursor(1, 0);
		LCD_displayCharacter(' ');
		_delay_ms(40);
		LCD_moveCursor(1, 0);
	}
}

int main(void) {
	Interrupt_init();
	LCD_init();

	while (1) {
		//print the first number
		if (flag_first) {
			second = 0;
			first = result;
			flag_first = 0;
			LCD_clearScreen();
			LCD_intgerToString(first);
			_delay_ms(40);
		} else {
			input = KEYPAD_getPressedKey();
			count1 = 0, second = 0, operator = 0;
			if (flag_op) {}
			else {first = 0;	}
			while ((input != '+') && (input != '-') && (input != '*') && (input != '/') && (input != '=')) {
				if (input == 'C') {
					LCD_clearScreen();
					_delay_ms(40);
					first = 0;
					count1 = 0;
				} else {
					first = first * 10 + (input - '0');
					LCD_displayCharacter(input);
					_delay_ms(40);
					count1++;
				}
				input = KEYPAD_getPressedKey();
			}
		}
		operator = input;
		if (operator == '=') {
			result = first;
			LCD_clearScreen();
			LCD_displayString("The result : ");
			LCD_moveCursor(1, 0);
			LCD_intgerToString(result);
			_delay_ms(40);
			input = KEYPAD_getPressedKey();
			if ((input == '+') | (input == '-') | (input == '*')
					| (input == '/')) {
				flag_first = 1;
			} else if ((input != '+') && (input != '-') && (input != '*')
					&& (input != '/')) {
				LCD_clearScreen();
			}
			continue;
		}
		LCD_moveCursor(1, 0);
		count2 = 0;
		//print the operator
		while ((input == '+') | (input == '-') | (input == '*') | (input == '/')) {
			LCD_moveCursor(1, 0);
			operator = input;
			LCD_displayCharacter(operator);
			_delay_ms(40);
			input = KEYPAD_getPressedKey();
		}
		flag_op = 0;
		count2++;
		LCD_moveCursor(1, 1);
		if (input == 'C') {
			LCD_clearScreen();
			_delay_ms(40);
			continue;
		}
		//print the second number
		while ((input != '+') && (input != '-') && (input != '*')&& (input != '/') && (input != '=')) {
			if (input == 'C') {
				flag_no_sec = 1;
				LCD_clearScreen();
				_delay_ms(40);
				break;
			}
			second = second * 10 + (input - '0');
			LCD_displayCharacter(input);
			flag_next_num = 1;
			count2++;
			_delay_ms(40);
			input = KEYPAD_getPressedKey();
		}
		if (flag_op) {continue;}
		result = operation(first, second, operator);
		if (flag_div) {
			flag_div = 0;
			LCD_clearScreen();
			LCD_displayString("Division by zero!");
			LCD_moveCursor(1, 0);
			LCD_displayString("Please,try again.");
			_delay_ms(40);
			continue;
		} else if (flag_no_sec) {
			flag_no_sec = 0;
			LCD_clearScreen();
			_delay_ms(40);
			continue;
		}
		if (input == '=') {
			LCD_clearScreen();
			LCD_displayString("The result : ");
			LCD_moveCursor(1, 0);
			LCD_intgerToString(operation(first, second, operator));
			_delay_ms(40);
			input = KEYPAD_getPressedKey();
			if ((input != '+') && (input != '-') && (input != '*')&& (input != '/')) {LCD_clearScreen();}
		}
		if ((input == '+') | (input == '-') | (input == '*') | (input == '/')) {
			flag_first = 1;
		}
	}
}
