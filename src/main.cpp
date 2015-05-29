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

int main() {

	Robot robot("localhost", 6665);
	Manager manager(&robot);
	manager.Run();

	return 0;
}
