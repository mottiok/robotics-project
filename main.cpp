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
#include "MapSearchNode.h"
#include "PathPlanner.h"

int main() {

//	Robot robot("localhost", 6665);
//	ObstacleAvoidancePlan plan(&robot);
//	Manager manager(&robot, &plan);
//	manager.Run();

	CMap map;

	map.SetResolutions(2.5, 10);
//	map.LoadMap("hospital_section.png");
        map.LoadMap("robotic_lab_map.png");

        /* TODO: Need a bit of refactoring for the coordinate/position struct
         * On the one hand, the struct called "SPosition" on the other hand it
         * used as coordinate. Maybe we should rename the struct to SCoordinate?
         */
        
        // Usage Example:
        
        // Real world positions
	dword startX = 362, startY = 305;
	dword goalX = 169, goalY = 138;
        
        // Convert to start and goal cells in our map
        SPosition startCellPos = map.PixelCoordToCellPosition(startX, startY);
        SPosition goalCellPos = map.PixelCoordToCellPosition(goalX, goalY);
        
//        map.ColorCellByCoord(startCellPos.dwX, startCellPos.dwY, 255, 0, 0);
//        map.ColorCellByCoord(goalCellPos.dwX, goalCellPos.dwY, 0, 255, 0);
//        map.DumpMap("debug.png");
//        return 0;

        // Create planner and find the best path from start to goal position
	PathPlanner planner(&map);
	vector<MapSearchNode*> nodesFromStartToGoal;
	bool searchState = planner.GetPathByStartAndGoalPosition(startCellPos,
                goalCellPos, nodesFromStartToGoal);
	
        // Dump map with a-star expansion and colored path from start (red) to goal (green)
        map.DumpMap("a_star_path.png");
        
        if (searchState) {
            // TODO:
            // Do something with 'nodesFromStartToGoal' and extract best waypoints
            // to guide the robot as smooth as possible using WaypointManager
        }

	return 0;
}
