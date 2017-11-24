/*
 * HardwareAbstractionLayer.cpp
 *
 *  Created on: 16.10.2017
 *      Author: abj240
 */

#include "HardwareLayer.h"

namespace hardwareLayer {

HardwareLayer::HardwareLayer() :
serial(signalGenerator),
_motor(actuators::Motor::instance()),
_switchPoint(actuators::SwitchPoint::instance()),
_trafficLight(mmi::TrafficLight::instance()),
_heightSensor(sensors::HeightSensor::instance()),
_ButtonLEDs(mmi::ButtonLEDs::instance())
{
	LOG_SCOPE;

	motorStop();
	motorRotateClockwise();
	switchPointClose();
	greenLightOff();
	redLightOff();
	yellowLightOff();
}

HardwareLayer::~HardwareLayer() {
	LOG_SCOPE;

	motorStop();
	switchPointClose();
	greenLightOff();
	redLightOff();
	yellowLightOff();

}

void HardwareLayer::motorStart() {
	_motor.start();
}

void HardwareLayer::motorStop() {
	_motor.stop();
}

void HardwareLayer::motorSlow() {
	_motor.setSlow();
}

void HardwareLayer::motorFast() {
	_motor.clearSlow();
}

void HardwareLayer::motorRotateClockwise() {
	_motor.setClockwiseRotation();
}

void HardwareLayer::motorRotateCounterclockwise() {
	_motor.setCounterclockwiseRotation();
}

void HardwareLayer::switchPointOpen(){
	_switchPoint.open();
}

void HardwareLayer::switchPointClose(){
	_switchPoint.close();
}

void HardwareLayer::greenLightOn(){
	_trafficLight.greenLightOn();
}

void HardwareLayer::greenLightOff(){
	_trafficLight.greenLightOff();
}

void HardwareLayer::yellowLightOn(){
	_trafficLight.yellowLightOn();
}

void HardwareLayer::yellowLightOff(){
	_trafficLight.yellowLightOff();
}

void HardwareLayer::redLightOn(){
	_trafficLight.redLightOn();
}

void HardwareLayer::redLightOff(){
	_trafficLight.redLightOff();
}

void HardwareLayer::blinkGreen(Speed speed) {
	_trafficLight.blinkGreen(speed);
}

void HardwareLayer::blinkYellow(Speed speed) {
	_trafficLight.blinkYellow(speed);
}

void HardwareLayer::blinkRed(Speed speed) {
	_trafficLight.blinkRed(speed);
}

void HardwareLayer::StartLEDOn(){
	_ButtonLEDs.LEDStartOn();
}

void HardwareLayer::StartLEDOff(){
	_ButtonLEDs.LEDStartOff();
}

void HardwareLayer::ResetLEDOn(){
	_ButtonLEDs.LEDResetOn();
}

void HardwareLayer::ResetLEDOff(){
	_ButtonLEDs.LEDResetOff();
}

void HardwareLayer::Q1LEDOn(){
	_ButtonLEDs.LEDQ1On();
}

void HardwareLayer::Q1LEDOff(){
	_ButtonLEDs.LEDQ1Off();
}

void HardwareLayer::Q2LEDOn(){
	_ButtonLEDs.LEDQ2On();
}
void HardwareLayer::Q2LEDOff(){
	_ButtonLEDs.LEDQ2Off();
}

Signal HardwareLayer::getSignal() {
	return signalGenerator.nextSignal();
}

void HardwareLayer::clearSignalBuffer() {
	signalGenerator.clearSignalBuffer();
}

void HardwareLayer::sendSerial(Signal signal) {
	serial.send(signal);
}

void HardwareLayer::sendItemViaSerial(Item* item) {
	serial.send(item);
}

Item HardwareLayer::getPassedItem() {
	return serial.getReceiver().getItemBuffer().pullItem();
}

int HardwareLayer::getItemBufferSize(){
	return serial.getReceiver().getItemBuffer().size();
}

uint16_t HardwareLayer::getHeight() {
	return _heightSensor.getHeight();
}

io::SignalGenerator& HardwareLayer::getSignalGenerator(){
	return this->signalGenerator;
}

} /* hardwareLayer */
