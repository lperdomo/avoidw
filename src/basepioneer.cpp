//============================================================================
// Name        : basepioneer.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include "ClassRobo.h"
using namespace std;

int main()
{
	int sucesso;
	PioneerRobot *robo;
	PioneerRobot(ConexaoSimulacao,"",&sucesso);
	//PioneerRobot(ConexaoRadio,"192.168.1.11",&sucesso);
	robo->Move(50,50);

	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
