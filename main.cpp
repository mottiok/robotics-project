//============================================================================
// Name        : robotics-project.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

#include "Debug.h"
#include "Configuration.h"
#include "Map.h"
#include "PathPlanner.h"
#include "WaypointManager.h"
#include "LocalizationManager.h"
#include "Robot.h"
#include "Plans/ObstacleAvoidancePlan.h"
#include "Manager.h"

double random(double dMin, double dMax) {
	double num = (double)rand() / RAND_MAX;
	return dMin + num * (dMax - dMin);
}

int main() {
	
	srand (time(NULL));
	
	/*
	// ---------------- TESTING START HERE ----------------
	PlayerClient pc("localhost", 6665);
	Position2dProxy pp(&pc);
	pp.SetMotorEnable(true);
	
	// starting point for the simulator
	double dX = 2.17;
	double dY = -2.88;
	double dYaw = 0;
	
	pp.SetOdometry(dX, dY, dYaw);
	
	for (int i=0; i<15; i++)
		pc.Read();
	
	double tempX, tempY, tempYaw = 0;
	pp.SetSpeed(0.2, 0);
	
	while (true) {
		tempX = pp.GetXPos(), tempY = pp.GetYPos(), tempYaw = pp.GetYaw();
//		printf("Delta: %lg, %lg, %lg\n", (tempX - dX), (tempY - dY), (tempYaw - dYaw)); // delta position
		printf("Position: %lg, %lg, %lg\n", tempX, tempY, tempYaw); // raw position
		dX = tempX, dY = tempY, dYaw = tempYaw;
		pp.SetOdometry(tempX, tempY, tempYaw + random(-1, 2)); // simulate the problem
//		pp.SetOdometry(tempX, tempY, tempYaw);
		pc.Read();
	}
	
	return 0;
	// ---------------- TESTING END HERE ----------------
	*/
	Configuration config;
	config.LoadConfigFile("parameters.txt");
	
	CMap map;
	map.SetResolutions(config.GetMapResolution(), config.GetGridResolution());
	map.LoadMap(config.GetMapFilePath());

	SDL2Wrapper sdl;
	sdl.CreateWindow("World Map", map.GetMapWidth(), map.GetMapHeight());

	// Convert to start and goal cells in our map
	SPosition startCellPos = map.PixelCoordToCellPosition(config.GetStartX(), config.GetStartY());
	SPosition goalCellPos = map.PixelCoordToCellPosition(config.GetGoalX(), config.GetGoalY());
	
	PathPlanner planner(&map);
	vector<MapSearchNode*> nodesFromStartToGoal;
	bool searchState = planner.GetPathByStartAndGoalPosition(startCellPos, goalCellPos, nodesFromStartToGoal);
	
	if (searchState) {
#ifdef DRAW_PATH
		for (int i=0; i < nodesFromStartToGoal.size(); i++) {
			map.ColorCellByCoord(nodesFromStartToGoal[i]->GetXPos(), nodesFromStartToGoal[i]->GetYPos(), BLUE_RGB_FORMAT);
		}
		
		map.DumpMap("a_star_path.png");
#endif
            
		WaypointManager WpMgr;

		if (!WpMgr.SetPath(nodesFromStartToGoal, DEFAULT_WAYPOINT_RESOLUTION, DEFAULT_WAYPOINT_ACCURACY))
		{
			printf("Failed to calculate waypoints...\n");
			return 0;
		}
		
		WpMgr.InitWaypointTraversal();
		
#ifdef DRAW_WAYPOINTS
		Waypoint* pCurrWP = WpMgr.GetStartWaypoint();

		do
		{
			printf("Waypoint at (%u, %u)\n", pCurrWP->GetXPos(), pCurrWP->GetYPos());
			map.ColorCellByCoord(pCurrWP->GetXPos(), pCurrWP->GetYPos(), YELLOW_RGB_FORMAT);
		}
		while (NULL != (pCurrWP = WpMgr.TraverseWaypoint()));

		map.DumpMap("waypoint_path.png");
#endif
		
		map.DumpMap("processed_map.png");
		sdl.LoadBackground("processed_map.png", true);
		
		LocalizationManager localization(&map, &sdl);
		Robot robot("localhost", 6665, &localization);
		robot.SetOdometryByPixelCoord(config.GetStartX(), config.GetStartY(), config.GetStartYaw(),
				map.GetPixelResolution(), map.GetMapWidth(), map.GetMapHeight());
		ObstacleAvoidancePlan plan(&robot, &WpMgr, map.GetPixelResolution(), map.GetGridResolution(), map.GetMapWidth(), map.GetMapHeight());
		
		Manager manager(&robot, &plan, &WpMgr, &map, &sdl);
		manager.Run();
	}
	
	return 0;
}
