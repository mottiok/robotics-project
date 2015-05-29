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
#include "Plans/ObstacleAvoidancePlan.h"
#include "Map.h"

int main() {

	Robot robot("localhost", 6665);
	Map map(&robot, "/usr/robotics/PcBotWorld/hospital_section.png");
	ObstacleAvoidancePlan plan(&robot);
	Manager manager(&robot, &plan, &map);
	manager.Run();

	return 0;
}
