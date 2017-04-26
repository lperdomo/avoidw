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

	int sensores[10];

	//robot.setDeltaHeading(degrees);
	robot.setRotVelMax(10);
	robot.enableMotors();
	while (Aria::getRunning()) {
		robot.lock();

		for (int i=0;i<10;i++) sensores[i]=(int)(robot.getSonarRange(i));
		cout << "1" << " " << sensores[0];
		cout << " 2" << " " << sensores[1];
		cout << " 3" << " " << sensores[2];
		cout << " 4" << " " << sensores[3];
		cout << " 5" << " " << sensores[4];
		cout << " 6" << " " << sensores[5];
		cout << " 7" << " " << sensores[6];
		cout << " 8" << " " << sensores[7];
		cout << " 9" << " " << sensores[8];
		cout << " 10" << " " << sensores[9] << endl;

		/*if (sensores[3] < 2500 && sensores[5] > 2500) {
			robot.setVel2(500, 100);
		} else if (sensores[3] < 2500 && sensores[0] > 2500) {
			robot.setVel2(100, 500);
		} else {
			robot.setVel2(500, 500);
		}*/


		robot.unlock();
		ArUtil::sleep(100);
	}

	Aria::exit(0);
	return 0;
}
