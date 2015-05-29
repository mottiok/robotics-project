//============================================================================
// Name        : robotics-project.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

#include "Robot.h"
#include "Manager.h"
#include "ObstacleAvoidancePlan.h"

int main() {

	Robot robot("localhost", 6665);
	ObstacleAvoidancePlan plan(&robot);
	Manager manager(&robot, &plan);
	manager.Run();

	return 0;
}
