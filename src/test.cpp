#include <SoftwareSerial.h>
SoftwareSerial wifiSerial(6, 7);

#include <SmartEMSChannel.h>
SmartEMSChannel channel1(5, 4, 10);
SmartEMSChannel channel2(3, 2, 9);

#include <WiFlyHQ.h>
#include <SPI.h>

#include <stdio.h>
#include <string.h>
#include <McpDigitalPot.h>
#include <EMSChannel.h>


// Blank due to security reasons
const char ssid[] = "Hci-Fi-2";
const char password[] = "forpeoplevery";

WiFly wifly;


#define SDI 10
#define SDI2 9

#define OPT1_1 5
#define OPT1_2 4

#define OPT2_1 3
#define OPT2_2 2

#define BAUD_RATE 115200

enum MessageType{
  UNDEFINED,
  INTENSITY,
  CHANNEL,
  STEPTIME,
  ONTIME
};


void setupWifi(){
    wifiSerial.begin(9600);

    if(!wifly.begin(&wifiSerial, &Serial)){
      Serial.println("Failed to start wifly");
    }

    if (!wifly.isAssociated()) {
	/* Setup the WiFly to connect to a wifi network */
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

    /* Setup for UDP packets, sent automatically */
    wifly.setIpProtocol(WIFLY_PROTOCOL_UDP);

    // Send UDP packet to this server and port
    wifly.setHost("192.168.3.9", 45454);

    wifly.setDeviceID("Wifly-UDP");

    Serial.println("Connected");
}


void setup()
{
	SPI.begin();

	channel1.setStepTime(10);
	channel1.setIntensityChangeFactor(1);
	channel1.on();

	channel2.setStepTime(10);
	channel2.setIntensityChangeFactor(1);
	channel2.on();

    Serial.begin(BAUD_RATE);
    Serial.println("Ready for Eclipse");

    channel2.setIntensity(127);
}


void sendMessage(char* msg){
   wifly.println(msg);
}

bool splitMessage(String message){
	int i = message.indexOf('I');
	if(i == -1)i = message.indexOf('i');
    if(i == -1)return false;

    int c = message.indexOf('C');
    if(c == -1) c = message.indexOf('c');
    if(c == -1) return false;

    int s = message.indexOf('S');
    if(s == -1) s = message.indexOf('s');
    if(s == -1) return false;

    int t = message.indexOf('T');
    if(t == -1) t = message.indexOf('t');
    if(t == -1) return false;

    if(!((i < c) && (c < s) && (s < t))) return false;
    int iValue = message.substring(i+1, c).toInt();
    int cValue = message.substring(c+1, s).toInt();
    int sValue = message.substring(s+1, t).toInt();
    int tValue = message.substring(t+1, message.length()).toInt();

   // Serial.println(iValue);
  //  Serial.println(cValue);
  // Serial.println(sValue);
  //  Serial.println(tValue);

    if(cValue == 0){
    	Serial.println("Channel 1 changed:");
    	channel1.setIntensity(iValue);
    	channel1.setStepTime(sValue);
    }else if(cValue == 1){
    	Serial.println("Channel 2 changed:");
    	channel2.setIntensity(iValue);
    	channel2.setStepTime(sValue);
    }else{
    	Serial.println("Selected Channel is not available in this sketch.");
    }

	Serial.print("Intensity:\t");
	Serial.println(iValue);
	Serial.print("StepTime:\t");
	Serial.println(sValue);



	return true;
}


bool increase = true;
void loop()
{
    // prints messages send to the arduino board

	/*
	if(wifly.available()){
		String udpPackage = "";
		while(wifly.available()){
			udpPackage += (char)wifly.read();
			delay(10);
		}
		delay(200);
		//Serial.println(udpPackage);
		splitMessage(udpPackage);
	}
	*/



    //splitMessage();
    if(increase){
    	channel2.increase();
	}else{
    	channel2.decrease();
    }

    if(channel2.getIntensity() >= 127)
    	increase = false;
    if(channel2.getIntensity() == 0)
    	increase = true;

    // only send messages to the user after user input
    if (Serial.available()) {
        const char c = Serial.read();


        if (c == '1') {
        	channel2.start();
        } else if (c == '2') {
        	channel2.off();
        } else if (c == '3') {
        }
    }

    channel2.update();
}
