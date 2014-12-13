

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
int const MAX_SERVO_ANGLE = 180;
int const MAX_DC_SPEED = 100;

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
			executeCommandWithCheck(command);
		}
	}

	// delay for loop
	delay(1000);

}

// OTHER METHODS

void executeCommandWithCheck(char* c){
	
	// check that command length is correct
	size_t len = strlen(c);
	if (len > 1 && len < 7){
		executeCommand(c);
	}
}

void executeCommand(char* c){

	// execute command
	if (c[0] == '1'){
		turnLedON(LED_GREEN);
	}
	else{
		turnLedOFF(LED_GREEN);
	}

	/*
	
	command list

	servo 1 - 00 1(smer vrtenja) XYZ (kot ~ 0-180)
	servo 2 - 01 1(smer vrtenja) XYZ (kot ~ 0-180)
	DC 1 - 10 1/0 (smer vrtenja ~ 1 - naprej, 0 - nazaj) XYZ (hitrost ~ 0-100)
	DC 2 - 11 1/0 (smer vrtenja ~ 1 - naprej, 0 - nazaj) XYZ (hitrost ~ 0-100)
	
	*/

	char motorType = c[0];	// motor type
	char motorNumber = c[1];	// motor number
	char direction = c[2];	// drive direction
	
	/*
	* NOT TESTED YET
	*/
	char* val;
	strncpy(val, c + 3, 3);
	val[4] = '\0';
	int angleOrSpeed = atoi(val);

	if (motorType == '0'){
		
		/*
		* SERVO motor
		*/

		setServoAngle(motorNumber, angleOrSpeed);

	}
	else if (motorType == '1'){
		
		/*
		* DC motor
		*/

		setDcSpeed(motorNumber, direction, angleOrSpeed);

	}


}

void turnLedON(int pin){
	digitalWrite(pin, HIGH);
}

void turnLedOFF(int pin){
	digitalWrite(pin, LOW);
}

void setServoAngle(char motorNum, int val){
	// set servo angle to specific value

	// check val
	if (val > MAX_SERVO_ANGLE)
		val = MAX_SERVO_ANGLE;

	if (motorNum == '0'){
		// servo 1
	}
	else if (motorNum = '1'){
		// servo 2
	}

}

void setDcSpeed(char motorNum, char direction, int val){
	// set DC speed and direction

	// check val
	if (val > MAX_DC_SPEED)
		val = MAX_DC_SPEED;

	if (motorNum == '0'){
		// DC 1
	}
	else if (motorNum = '1'){
		// DC 2
	}
}

