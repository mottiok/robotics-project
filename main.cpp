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

int main() {
	
	CMap map;
	map.SetResolutions(2.5, 10);
	map.LoadMap("robotic_lab_map.png");

	SDL2Wrapper sdl;
	sdl.CreateWindow("World Map", map.GetMapWidth(), map.GetMapHeight());
	map.DrawMapState(&sdl, true);
	
	dword startX = 362, startY = 305, startYaw = 0;
	dword goalX = 169, goalY = 138;

	// Convert to start and goal cells in our map
	SPosition startCellPos = map.PixelCoordToCellPosition(startX, startY);
	SPosition goalCellPos = map.PixelCoordToCellPosition(goalX, goalY);
	
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
		
		LocalizationManager localization(&map, &sdl);
		Robot robot("localhost", 6665, &localization);
		robot.SetOdometryByPixelCoord(startX, startY, startYaw, map.GetPixelResolution(), map.GetMapWidth(), map.GetMapHeight());
		ObstacleAvoidancePlan plan(&robot, &WpMgr, map.GetPixelResolution(), map.GetMapWidth(), map.GetMapHeight());
		
		Manager manager(&robot, &plan, &WpMgr, &map, &sdl);
		manager.Run();
	}
	
	return 0;
}
