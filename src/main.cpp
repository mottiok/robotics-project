//============================================================================
// Name        : robotics-project.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

#include <libplayerc++/playerc++.h>
using namespace PlayerCc;

int main() {

	PlayerClient pc("localhost", 6665);
	LaserProxy lp(&pc);
	Position2dProxy pp(&pc);

	pp.SetMotorEnable(true);
	while (true) {
		pc.Read();

		if (lp[333] < 0.8)
			pp.SetSpeed(0.0, 0.3);
		else
			pp.SetSpeed(0.8, 0.0);
	}

	return 0;
}
