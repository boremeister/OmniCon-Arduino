

/*

	BlueMotor ver 1
	Author: boremeister
	Date: 19.10.2014 - 8.12.2014

*/

#include <Servo.h>
#include <SoftwareSerial.h>		// http://arduino.cc/en/Reference/SoftwareSerial

// -------------------------- prepare --------------------------

#define DEBUG_MODE 1					// DEBUG_MODE mode (1 - on, 0 - off)
#define RX_PIN 7						// Bluetooth TX pin (HC-05 TX)
#define TX_PIN 8						// Bluetooth RX pin (HC-05 RX)
#define LED_GREEN 13					// LED Green
#define SERVO_1 9						// Servo 1
#define SERVO_2 10						// Servo 2

// SoftwareSerial - BlueTooth HC-05
SoftwareSerial BTSerial(RX_PIN,TX_PIN);	// atmega RX / TX

// Servos
Servo servo1;
//Servo servo2;

// other variables
String response;
String startString = "~", endString = "#";
char endChar = '#';
int const MAX_SERVO_ANGLE = 180;
int const MAX_DC_SPEED = 100;
int servo1pos = 0;
int servo2pos = 0;

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

	// servos
	servo1.attach(SERVO_1);
	//servo2.attach(SERVO_2);

	// set servos to default position
	//servoTest(servo1);

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
	//delay(1000);

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

	servo 1 - 0 1(smer vrtenja) XYZ (kot ~ 0-180)
	servo 2 - 1 1(smer vrtenja) XYZ (kot ~ 0-180)
	DC 1 - 2 1/0 (smer vrtenja ~ 1 - naprej, 0 - nazaj) XYZ (hitrost ~ 0-100)
	DC 2 - 3 1/0 (smer vrtenja ~ 1 - naprej, 0 - nazaj) XYZ (hitrost ~ 0-100)
	
	*/

	char motor = c[0];	// motor type
	char direction = c[1];	// drive direction
	
	/*
	* get angle/speed
	*
	* The character encodings for digits in AscII are all in order from 48 (for '0') to 57 (for '9').
	* The integer value for any digit is the digit less 48 ('0' is 48).
	*/
	int enka = c[2] - '0';
	int dvojka = c[3] - '0';
	int trojka = c[4] - '0';
	int angleOrSpeed = enka * 100 + dvojka * 10 + trojka;

	switch (motor){
		case '0':

			/*
			* SERVO motor 1
			*/

			setServoAngle(motor, angleOrSpeed);

			break;

		case '1':

			/*
			* SERVO motor 2
			*/

			setServoAngle(motor, angleOrSpeed);

			break;

		case '2':

			/*
			* DC motor 1
			*/

			setDcSpeed(motor, direction, angleOrSpeed);

			break;

		case '3':

			/*
			* DC motor 2
			*/

			setDcSpeed(motor, direction, angleOrSpeed);

			break;

		default:
			break;
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
		servo1.write(val);
		delay(10);
	}
	else if (motorNum = '1'){
		// servo 2
		//servo2.write(val);
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

/*
* servo motor test - sweep (http://arduino.cc/en/Tutorial/sweep)
*/
void servoTest(Servo servo){
	int pos;
	for (pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
	{                                  // in steps of 1 degree 
		servo.write(pos);              // tell servo to go to position in variable 'pos' 
		delay(15);                       // waits 15ms for the servo to reach the position 
	}
	for (pos = 180; pos >= 1; pos -= 1)     // goes from 180 degrees to 0 degrees 
	{
		servo.write(pos);              // tell servo to go to position in variable 'pos' 
		delay(15);                       // waits 15ms for the servo to reach the position 
	}
}

