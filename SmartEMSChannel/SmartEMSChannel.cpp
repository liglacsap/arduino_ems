/*
 * SmartEMSChannel.cpp
 *
 *  Created on: 02.06.2014
 *      Author: gil
 */

#include "SmartEMSChannel.h"

SmartEMSChannel::SmartEMSChannel(uint8 pinResistor, uint8 pinPad, uint8 pinPotentiometer) {
	potentiometer = new McpDigitalPot(pinPotentiometer, 5000);

	pinMode(pinResistor, OUTPUT);
	pinMode(pinPad, OUTPUT);

	lastTimeStamp = millis();

	this->pinResistor = pinResistor;
	this->pinPad = pinPad;

	onTime = 0;
	realIntensity = intensity = 0;

	on();
}

SmartEMSChannel::~SmartEMSChannel() {
	delete(potentiometer);
}

void SmartEMSChannel::on(){
	digitalWrite(pinPad, HIGH);
	digitalWrite(pinResistor, LOW);
}

void SmartEMSChannel::off(){
	digitalWrite(pinPad, LOW);
	digitalWrite(pinResistor, HIGH);
}

uint SmartEMSChannel::getState(){
	return state;
}

uint SmartEMSChannel::getIntensity(){
	return intensity;
}

uint SmartEMSChannel::getRealIntensity(){
	return realIntensity;
}

void SmartEMSChannel::setIntensity(uint intensity){
	this->intensity = intensity;
	lastTimeStamp = millis();

	if(intensity > realIntensity)
		calculatedStepTime = minChangeTime / (intensity - realIntensity);
	else
		calculatedStepTime = minChangeTime / (realIntensity - intensity);
}

void SmartEMSChannel::setRealIntensity(uint intensity){
	realIntensity = (intensity <= 127) ? intensity : 127;
	realIntensity = (realIntensity >= 0) ? realIntensity : 0;
	realIntensity = intensity;

	potentiometer->writePosition(0, realIntensity);

	Serial.print(intensity);
	Serial.print("  ");
	Serial.print(potentiometer->getPosition(0));
	Serial.print("\n");

}
uint SmartEMSChannel::getOffTime(){
	return offTime;
}
void SmartEMSChannel::setOffTime(uint offTime){
	this->offTime = offTime;
	calculateFrequency();
}
void SmartEMSChannel::calculateFrequency(){
	frequency = minChangeTime * 2 + onTime + offTime;
}

uint SmartEMSChannel::getMinChangeTime(){
	return minChangeTime;
}

void SmartEMSChannel::setMinChangeTime(uint minChangeTime){
	this->minChangeTime = minChangeTime;

	if(intensity > realIntensity)
		calculatedStepTime = minChangeTime / (intensity - realIntensity);
	else
		calculatedStepTime = minChangeTime / (realIntensity - intensity);

	calculateFrequency();
}

void SmartEMSChannel::increase(){
	uint start = millis();
	setRealIntensity(realIntensity + 1);


	uint timePassed = millis() - start;

	if(calculatedStepTime > timePassed)
	delay(calculatedStepTime - timePassed);
}

void SmartEMSChannel::decrease(){
	uint start = millis();
	setRealIntensity(realIntensity - 1);

	uint timePassed = millis() - start;

	if(calculatedStepTime > timePassed)
	delay(calculatedStepTime - timePassed);
}

void SmartEMSChannel::setOnTime(uint time){
	onTime = time;
	calculateFrequency();
}

void SmartEMSChannel::start(){
	state = INCREASING;
	realIntensity = 0;

	lastTimeStamp = millis();
}



void SmartEMSChannel::update(){
	// check if the end of on time was reached
	if(millis() - onTimeStamp > onTime && onTime != 0){
		setIntensity(0);
	}

	if(realIntensity == intensity){
		onTimeStamp = millis();
		// Gewünschte Intensität erreicht
	}else{
		if(realIntensity < intensity){
			increase();
		}else if(realIntensity > intensity){
			decrease();
		}
	}
}
