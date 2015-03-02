#include <QTRSensors.h>

// create an object for four QTR-xRC sensors on digital pins 14,15,16,17 (analog inputs A0 - A3)
QTRSensorsRC qtrrc((unsigned char[]) {14, 15, 16, 17}, 4);

void setup()
{
	// Start calibration phase and move the sensors over both reflectance extremes to be encountered
	int i;
	for (i = 0; i < 250; i++)
	{
		qtrrc.calibrate();
		delay(20);
	}

	// Signal calibration is over
}

void OptoSensorReading()
{
	unsigned int sensors[4];
	qtrrc.readCalibrated(sensors);
	optoSensorStateA = sensors[0];
	optoSensorStateB = sensors[1];
	optoSensorStateC = sensors[2];
	optoSensorStateD = sensors[3];
}