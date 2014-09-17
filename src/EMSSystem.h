/*
 * EMSSystem.h
 *
 *  Created on: 16.06.2014
 *      Author: gil
 */

#ifndef EMSSYSTEM_H_
#define EMSSYSTEM_H_

#include "SmartEMSChannel.h"

#include <WiFlyHQ.h>
#include <SPI.h>
#include <SoftwareSerial.h>

#define MAX_EMS_CHANNEL 999;

enum Parameter{
	PARAM_UNKNOWN,
	PARAM_CHANNEL,
	PARAM_INTENSITY,
	PARAM_ON_TIME,
	PARAM_DEVICE,
	PARAM_IP,
	PARAM_SSID,
	PARAM_SYNC,
	PARAM_ON_OFF,
	PARAM_FREQUENCY,
	PARAM_MIN_CHANGE_TIME,
	PARAM_MIN_CALIBRATION,
	PARAM_MAX_CALIBRATION
};


namespace EMS{

	class System {
	private:
		SmartEMSChannel** channel;
		uint currentChannel;

		uint numChannel;

		WiFly wifly;
		SoftwareSerial* wifiSerial;

		const char* ip;

		String opt;
		String value;
		bool option;
		int parameter;


		void parseMessage();
	public:
		System(uint numChannel, uint pins[][3], uint pinWifly[2]);
		virtual ~System();

		void update();

		bool enableConnection(const char* serverIP, const char* ssid, const char* password);
		const char* getIPAddress();
	};

	#endif /* EMSSYSTEM_H_ */

}
