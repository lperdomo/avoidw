#include <iostream>
#include "Aria.h"

using namespace std;

int main(int argc, char** argv)
{
	Aria::init();
	ArArgumentParser parser(&argc, argv);
	parser.loadDefaultArguments();
	ArRobot robot;
	ArAnalogGyro gyro(&robot);
	ArSonarDevice sonar;
	ArSick sick;
	ArRobotConnector robotConnector(&parser, &robot);
	ArLaserConnector laserConnector(&parser, &robot, &robotConnector);

	if(!robotConnector.connectRobot()) {
		ArLog::log(ArLog::Terse, "Failed to connect with Pioneer");
		if(parser.checkHelpAndWarnUnparsed()) {
			Aria::logOptions();
			Aria::exit(1);
		}
	}
	ArLog::log(ArLog::Normal, "Connected with Pioneer");
	robot.addRangeDevice(&sonar);
	robot.addRangeDevice(&sick);
	robot.runAsync(true);

	sick.runAsync();
	laserConnector.setupLaser(&sick);
	if(!laserConnector.connectLaser(&sick)) {
		ArLog::log(ArLog::Terse, "Failed to connect with Sick");
		if(parser.checkHelpAndWarnUnparsed()) {
			Aria::logOptions();
			Aria::exit(1);
		}
	}

	int sonars[8];
	vector<ArSensorReading>* sickReadings;
	bool obstacleFront, obstacleLeft, obstacleRight
	, obstacleLeftFront, obstacleRightFront, obstacleFrontLeft, obstacleFrontRight;
	robot.enableMotors();
	robot.setVel2(200, 200);
	while (Aria::getRunning()) {
		robot.lock();

		for (int i=0;i<8;i++) sonars[i] = robot.getSonarRange(i);

		obstacleFront = (sonars[3] < 2000 || sonars[4] < 2000);
		obstacleLeft = (sonars[0] < 2000);
		obstacleRight = (sonars[7] < 2000);
		obstacleLeftFront = (sonars[1] < 2000);
		obstacleRightFront = (sonars[6] < 2000);
		obstacleFrontLeft = (sonars[2] < 2000);
		obstacleFrontRight = (sonars[5] < 2000);
		//covering sonar's blind spots
		sick.lockDevice();
		sickReadings = sick.getRawReadingsAsVector();
		if (sickReadings->size() > 0) {
			if (sickReadings->at(85).getRange() < 2000
			|| sickReadings->at(90).getRange() < 2000
			|| sickReadings->at(95).getRange() < 2000) {
				obstacleFront = true;
			}
		}
		sick.unlockDevice();

		if (!obstacleFront && !obstacleFrontLeft && !obstacleFrontRight) {
			robot.setVel2(200, 200);
		} else if (!obstacleFront && obstacleFrontLeft && !obstacleFrontRight) {
			robot.setVel2(25, 0);
		} else if (!obstacleFront && !obstacleFrontLeft && obstacleFrontRight) {
			robot.setVel2(0, 25);
		} else if (obstacleFront && !obstacleFrontLeft && !obstacleLeftFront) {
			robot.setVel2(0, 25);
		} else if (obstacleFront && !obstacleFrontRight && !obstacleRightFront) {
			robot.setVel2(25, 0);
		} else if (obstacleFront && !obstacleFrontLeft && !obstacleFrontRight && !obstacleFrontLeft && !obstacleFrontRight) {
			robot.setVel2(0, 10);
		} else if (obstacleFront && obstacleFrontLeft && obstacleFrontRight && !obstacleLeft) {
			robot.setVel2(0, 50);
		} else if (obstacleFront && obstacleFrontLeft && obstacleFrontRight && !obstacleRight) {
			robot.setVel2(50, 0);
		}

		robot.unlock();
		ArUtil::sleep(100);
	}

	Aria::exit(0);
	return 0;
}
