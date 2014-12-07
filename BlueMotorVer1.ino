

/*

	BlueMotor ver 1
	Author: Borko Horvat
	Date: 19.10.2014 - 7.12.2014

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
String startChar = "~", endChar = "#";

// char buffer
//char command[6];

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
	
	// check if BT is availiable
	if(BTSerial.available()){

		char ch = BTSerial.read();

		// build response message
		response = startChar + "Received: " + ch + endChar;
		BTSerial.println(response);

		// check which char received
		if(ch == 'x'){
			turnLedON(LED_GREEN);
		}else if (ch == 'y'){
			turnLedOFF(LED_GREEN);
		}else{
			// unknown command - do nothing
			//BTSerial.println("~Unknown command!#");
		}

		// small delay for data transfer - eliminate missed transmissions
		delay(10);
	}

	response = "";
	
	/*
	// READ STRING

	// position for command buffer
	int i = 0;

	while(BTSerial.available()){
		//small delay to allow input buffer to fill
		delay(10);

		char ch = BTSerial.read();	

		// check if end
		if(ch == 'e'){
			BTSerial.println("Read end of command!");
			command[i] = '\0';	// command end
			turnLedON(LED_GREEN);
			break;
		} else {
			BTSerial.println("Read ch: " + ch);
			command[i] = ch;
			i = i + 1;
		}
	}
	*/


	// delay for loop
	delay(1000);
}

// helper methods
void turnLedON(int pin){
	digitalWrite(pin, HIGH);
}

void turnLedOFF(int pin){
	digitalWrite(pin, LOW);
}