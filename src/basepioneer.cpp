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
	ArRobotConnector robotConnector(&parser, &robot);
	ArLaserConnector laserConnector(&parser, &robot, &robotConnector);

	if(!robotConnector.connectRobot()) {
		ArLog::log(ArLog::Terse, "Sem conexão com Pioneer");
		if(parser.checkHelpAndWarnUnparsed()) {
			Aria::logOptions();
			Aria::exit(1);
		}
	}
	ArLog::log(ArLog::Normal, "Conexão com Pioneer");
	robot.addRangeDevice(&sonar);
	robot.runAsync(true);

	ArKeyHandler keyHandler;
	Aria::setKeyHandler(&keyHandler);
	robot.attachKeyHandler(&keyHandler);
	cout << "ESC para sair" << endl;

	int sensores[8];

	robot.enableMotors();
	while (Aria::getRunning()) {
		robot.lock();

		for (int i=0;i<8;i++) sensores[i]=(int)(robot.getSonarRange(i));

		if (sensores[3] < 2500 && sensores[5] > 2500) {
			robot.setVel2(500, 100);
		} else if (sensores[3] < 2500 && sensores[0] > 2500) {
			robot.setVel2(100, 500);
		} else {
			robot.setVel2(500, 500);
		}
		robot.unlock();
		ArUtil::sleep(100);
	}

	Aria::exit(0);
	return 0;
}
