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

int main() {

	Robot robot("localhost", 6665);
	ObstacleAvoidancePlan plan(&robot);
	Manager manager(&robot, &plan);
	manager.Run();

	/*
	
	CMap map;

	map.SetResolutions(1,10);
	map.LoadMap("hospital_section.png");
	
	*/
	
	return 0;
}
