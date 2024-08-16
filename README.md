Simple Calculator using ATmega32
This project demonstrates the core functionalities of basic arithmetic operations: addition, subtraction, multiplication, and division.
The calculator accepts inputs from the keypad, processes the data using the ATmega32 microcontroller, and displays the results on a 16x2 LCD.
Each keypress is interpreted by the microcontroller to perform the required arithmetic operation. 
It can perform multiple operations and also operations on the results of the operation.
I also added a push button as an external interrupt so it can be used to delete one character if needed.

Hardware and software used:
Microcontroller: ATmega32 with frequency of 8Mhz
Input : 4x4 Keypad
Display : 16x2 LCD 
Drivers used : DIO, LCD, KEYPAD
Operations: Addition, Subtraction, Multiplication, Division
Interrupt Functionality: Implemented an interrupt to allow users to delete a character if they make a mistake during input.




 
