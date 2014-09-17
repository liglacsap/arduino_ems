/*
 * SmartEMSChannel.h
 *
 *  Created on: 02.06.2014
 *      Author: gil
 */

#ifndef SMARTEMSCHANNEL_H_
#define SMARTEMSCHANNEL_H_

#include <McpDigitalPot.h>
#include <Arduino.h>

typedef unsigned int uint;
typedef uint8_t uint8;
enum ActiveState{
	ON,
	OFF,
	INCREASING,
	DECREASING,
	ON_TIME
};

class SmartEMSChannel {
private:
	McpDigitalPot* potentiometer;

	uint8_t pinResistor;
	uint8_t pinPad;
	uint8_t pinPotentiometer;

	uint state;
	uint intensity;
	uint realIntensity;
	uint minChangeTime;
	uint onTime;
	uint offTime;

	uint intensityChangeFactor;
	uint calculatedStepTime;

	uint lastTimeStamp;
	uint onTimeStamp;

	uint frequency;
public:
	SmartEMSChannel(uint8 pinResistor, uint8 pinPad, uint8 pinPotentiometer);

	virtual ~SmartEMSChannel();

	void on();
	void off();
	uint getState();

	uint getIntensity();
	void setIntensity(uint intensity);

	uint getRealIntensity();

	void setRealIntensity(uint intensity);

	uint getMinChangeTime();
	void setMinChangeTime(uint changeTime);

	uint getOffTime();
	void setOffTime(uint offTime);

	void calculateFrequency();

	void increase();
	void decrease();

	void start();

	void update();

	uint getChannel();
	void setChannel(uint channel);

	void setOnTime(uint time);
};

#endif

