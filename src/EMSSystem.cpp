/*
 * EMSSystem.cpp
 *
 *  Created on: 16.06.2014
 *      Author: gil
 */

#include "EMSSystem.h"

using namespace EMS;

System::System(uint numChannel, uint pins[][3], uint pinWifly[2]) {

	channel = new SmartEMSChannel*[numChannel];
	for(uint i=0; i<numChannel; i++){
		channel[i] = new SmartEMSChannel(pins[i][0], pins[i][1], pins[i][2]);
		channel[i]->on();
	}


	this->numChannel = numChannel;

	currentChannel = 0;

	wifiSerial = new SoftwareSerial(pinWifly[0], pinWifly[1]);

	ip = "";

	opt = "";
	option = false;
	parameter = PARAM_UNKNOWN;
}

System::~System() {
	// TODO Auto-generated destructor stub
}

void System::parseMessage(){
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
					Serial.print("Intensity ");
					Serial.print(value);
					channel[currentChannel]->setIntensity(value.toInt());
					value = "";
					parameter = PARAM_UNKNOWN;
				}
				break;
			case PARAM_CHANNEL:
				if(b >= 48 && b <=57){
					value += b;
				}else{
					currentChannel = value.toInt();

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
					Serial.print("Channel ");
					Serial.print(currentChannel+1);

					if(value.toInt() == 0){
						channel[currentChannel]->off();
						Serial.print(" off\n");
					}else{
						channel[currentChannel]->on();
						Serial.print(" on\n");
					}

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
					Serial.print("Changed: MinChangeTime to ");
					Serial.print(value.toInt());
					Serial.print("\n");

					channel[currentChannel]->setMinChangeTime(value.toInt());

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
}

void System::update(){
	this->parseMessage();


	for(uint i=0; i<2; i++){
		channel[i]->update();
	}
}
const char* System::getIPAddress(){
	return ip;
}
bool System::enableConnection(const char* serverIP, const char* ssid, const char* password){
	wifiSerial->begin(9600);

    if(!wifly.begin(wifiSerial, &Serial)){
    	return false;
    }

    wifly.setFlushSize(4);
    wifly.setFlushTimeout(10);
    wifly.setRate(15);
    wifly.save();


    if (!wifly.isAssociated()) {
	/* Setup the WiFly to connect to a wifi network */
		Serial.println("Joining network");
		wifly.setSSID(ssid);
		wifly.setPassphrase(password);
		wifly.enableDHCP();



		if(wifly.join()){
		    char buf[32];
		    wifly.getIP(buf, sizeof(buf));
		    Serial.println(buf);
		    ip = buf;

		    /* Setup for UDP packets, sent automatically */
		    wifly.setIpProtocol(WIFLY_PROTOCOL_UDP);

		    // Send UDP packet to this server and port
		    wifly.setHost(serverIP, 454545);


		    wifly.setDeviceID("Wifly-UDP");



			return true;
		}else{
			return false;
		}
    }else{
	    char buf[32];
	    wifly.getIP(buf, sizeof(buf));
	    Serial.println(buf);
	    ip = buf;

    	return true;
    }
}
