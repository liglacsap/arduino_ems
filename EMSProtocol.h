/*
 * EMSProtocol.h
 *
 *  Created on: 16.06.2014
 *      Author: gil
 */

#ifndef EMSPROTOCOL_H_
#define EMSPROTOCOL_H_

#include <McpDigitalPot.h>
#include <Arduino.h>

typedef unsigned int uint;
typedef uint8_t uint8;



class EMSProtocol {
public:
	EMSProtocol();
	virtual ~EMSProtocol();

	static void setChannel(uint* channel);
	static void setIntensity(uint* intensity);
	static void setOnTime(uint* onTime);

	static void setProperty(String* identifier, uint* value);
	static void setIP(String* ip);
	static void setSSID(String* ssid);
	static void setSync(uint* channel);
	static void setOn();
	static void setOff();
	static void setFrequency(uint frequency);
	static void setMinimalChangeTime(uint minChangeTime);
	static void setMinimumCalibration(uint* channel, uint* intensity);
	static void setMaximumCalibration(uint* channel, uint* intensity);

	static void splitMessage();
};

#endif /* EMSPROTOCOL_H_ */
