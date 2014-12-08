

/*

	BlueMotor ver 1
	Author: boremeister
	Date: 19.10.2014 - 8.12.2014

*/

//#include <Servo.h>
#include <SoftwareSerial.h>		// http://arduino.cc/en/Reference/SoftwareSerial

// -------------------------- prepare --------------------------

#define DEBUG_MODE 1					// DEBUG_MODE mode (1 - on, 0 - off)
#define RX_PIN 7						// Bluetooth TX pin (HC-05 TX)
#define TX_PIN 8						// Bluetooth RX pin (HC-05 RX)
#define LED_GREEN 13					// LED Green
#define SERVO_ONE 12					// Servo one

// SoftwareSerial - BlueTooth HC-05
SoftwareSerial BTSerial(RX_PIN,TX_PIN);	// atmega RX / TX

// other variables
String response;
String startString = "~", endString = "#";
char endChar = '#';

// command array
char command[7];

void setup()
{
	// set serial and bluetooth serial
	//Serial.begin(9600);
	BTSerial.begin(9600);

	//BTSerial.flush();

	BTSerial.println("~Bluetooth ready!#");
	//Serial.println("Bluetooth ready!");

	// leds
	pinMode(LED_GREEN, OUTPUT);

	// default start delay
	delay(1000);
}

void loop()
{
	
	// READ / PREPARE COMMAND

	int i = 0;	// position for command buffer
	char *response = ""; // prepare response

	while(BTSerial.available()){
		
		//small delay to allow input buffer to fill
		delay(10);

		// read char from BT module
		char ch = BTSerial.read();	

		/*
			build command:
			* until end character received 
			* check that command is not too long
		*/

		if (ch != endChar && i < 6){

			// add received char into command*
			command[i] = ch;
			i = i + 1;
		}
		else {

			// command ended or too long
			command[i] = '\0';	// command end - string has to be "null terminated"
			strncpy(response, command, i+1);

			// send response
			BTSerial.println(startString + response + endString);

			// EXECUTE COMMAND
			executeCommand(command);
		}
	}

	// delay for loop
	delay(1000);

}

// OTHER METHODS

void executeCommand(char* c){

	// execute command
	if (c[0] == '1'){
		turnLedON(LED_GREEN);
	}
	else{
		turnLedOFF(LED_GREEN);
	}

}

void turnLedON(int pin){
	digitalWrite(pin, HIGH);
}

void turnLedOFF(int pin){
	digitalWrite(pin, LOW);
}