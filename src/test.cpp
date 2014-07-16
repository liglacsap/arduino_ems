#include "EMSSystem.h"

#define BAUD_RATE 115200

uint pins[2][3] = {{5, 4, 10}, {2, 3, 9}};
uint pinWifi[2] = {6, 7};

EMS::System* system;

void setup(){
	SPI.begin();

	Serial.begin(BAUD_RATE);
	system = new EMS::System(2, pins, pinWifi);
	if(system->enableConnection("Hci-Fi-2", "forpeoplevery")){
		Serial.println("Connection established, have fun");
	}else{
		Serial.println(":(");
	}
}

void loop(){
	system->update();
}


/*
#include <SoftwareSerial.h>
SoftwareSerial wifiSerial(6, 7);

#include <SmartEMSChannel.h>
SmartEMSChannel channel1(5, 4, 10);
SmartEMSChannel channel2(3, 2, 9);
SmartEMSChannel* currentChannel = &channel2;

#include <WiFlyHQ.h>
#include <SPI.h>

#include <stdio.h>
#include <string.h>
#include <McpDigitalPot.h>
#include <EMSSystem.h>

const char ssid[] = "Hci-Fi-2";
const char password[] = "forpeoplevery";

WiFly wifly;


#define SDI 10
#define SDI2 9

#define OPT1_1 5
#define OPT1_2 4

#define OPT2_1 3
#define OPT2_2 2



typedef unsigned char uchar;




void setupWifi(){
    wifiSerial.begin(9600);

    if(!wifly.begin(&wifiSerial, &Serial)){
      Serial.println("Failed to start wifly");
    }

    if (!wifly.isAssociated()) {
	Serial.println("Joining network");
	wifly.setSSID(ssid);
	wifly.setPassphrase(password);
	wifly.enableDHCP();

	if (wifly.join()) {
	    Serial.println("Joined wifi network");
	} else {
	    Serial.println("Failed to join wifi network");
	}
    } else {
        Serial.println("Already joined network");
    }

    char buf[32];
    wifly.getIP(buf, sizeof(buf));
    Serial.println(buf);

    wifly.setIpProtocol(WIFLY_PROTOCOL_UDP);

    // Send UDP packet to this server and port
    wifly.setHost("192.168.3.9", 45454);

    wifly.setDeviceID("Wifly-UDP");

    Serial.println("Connected");
}


void setup()
{
	SPI.begin();

	//channel1.setStepTime(10);
	channel1.on();

	//channel2.setStepTime(10);
	channel2.on();

    Serial.begin(BAUD_RATE);

    setupWifi();
    Serial.println("Ready for Eclipse");
}


void sendMessage(char* msg){
   wifly.println(msg);
}

bool increase = true;

bool option = false;
int parameter = -1;

String value;
String opt ="";
void loop()
{
	char b;
	if(wifly.available() > 0){
		b = (char)wifly.read();

		switch(parameter){
			case PARAM_UNKNOWN:
				break;
			case PARAM_INTENSITY:
				if(b >= 48 && b <=57){
					value += b;
				}else{
					Serial.println("SET I");
					currentChannel->setIntensity(value.toInt());
					value = "";
					parameter = PARAM_UNKNOWN;
				}
				break;
			case PARAM_CHANNEL:
				if(b >= 48 && b <=57){
					value += b;
				}else{
					if(value.toInt() == 0){
						currentChannel = &channel1;
					}else{
						currentChannel = &channel2;
					}

					value = "";
					parameter = PARAM_UNKNOWN;
				}
				break;
			case PARAM_ON_TIME:
				break;
			case PARAM_DEVICE:
				break;
			case PARAM_IP:
				break;
			case PARAM_SSID:
				break;
			case PARAM_SYNC:
				break;
			case PARAM_ON_OFF:
				if(b >= 48 && b <=57){
					value += b;
				}else{
					if(value.toInt() == 0)
						currentChannel->off();
					else
						currentChannel->on();

					value = "";
					parameter = PARAM_UNKNOWN;
				}
				break;
			case PARAM_FREQUENCY:
				break;
			case PARAM_MIN_CHANGE_TIME:
				if(b >= 48 && b <=57){
					value += b;
				}else{
					currentChannel->setMinChangeTime(value.toInt());

					value = "";
					parameter = PARAM_UNKNOWN;
				}

				break;
			case PARAM_MIN_CALIBRATION:
				break;
			case PARAM_MAX_CALIBRATION:
				break;
		}


		if(!option){
			if(b == 'I'){
				parameter = PARAM_INTENSITY;
			}
			if(b == 'C'){
				parameter = PARAM_CHANNEL;
			}
			if(b == 'T'){
				parameter = ON_TIME;
			}
			if(b == '_'){
				option = true;
			}
		}else{
			if(b >= 48 && b <=57){
				opt="";
				option = false;
			}else{
				opt+=b;
			}

			if(opt == "DE"){
				parameter = PARAM_DEVICE;
				opt="";
			}
			if(opt == "IP"){
				parameter = PARAM_IP;
				opt="";
			}
			if(opt == "SS"){
				parameter = PARAM_SSID;
				opt="";
			}
			if(opt == "OO"){
				parameter = PARAM_ON_OFF;
				opt="";
			}
			if(opt == "FR"){
				parameter = PARAM_FREQUENCY;
				opt="";
			}
			if(opt == "CT"){
				parameter = PARAM_MIN_CHANGE_TIME;
				opt="";
			}
			if(opt == "MI"){
				parameter = PARAM_MIN_CALIBRATION;
				opt="";
			}
			if(opt == "MA"){
				parameter = PARAM_MAX_CALIBRATION;
				opt="";
			}
		}



		// needed delay, see http://forum.arduino.cc/index.php/topic,48103.0.html
		// for more details
		delay(1);
	}



	currentChannel->update();

}
*/
