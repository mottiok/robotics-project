#ifndef WAYPOINT_MANAGER_H
#define WAYPOINT_MANAGER_H

#include "Configuration.h"
#include "Waypoint.h"
#include "typedefs.h"
#include "MapSearchNode.h"

#include <vector>
using namespace std;

class WaypointManager
{
	vector<Waypoint*> m_vWaypoints;
	Waypoint* m_pCurrWaypoint;

	Waypoint* LinkNextWaypoint(MapSearchNode* pNode, Waypoint* pLast);
	dword FindNextSignificantNode(vector<MapSearchNode*>& vPath, dword dwStartNode, dword dwLimitNode, dword dwResolution, float fTestGradient, float fAccuracy);
	
public:
	WaypointManager() : m_pCurrWaypoint(NULL) {};
	~WaypointManager();

	// Calculates a sequence of waypoints given a vector of location nodes as a path
	// vPath 	    - path as vector of nodes
	// dwResolution	    - Amount of cells to move between each path sample
	// dwLinearAccuracy - Threshold value to determine if a gradient difference between
	//			two following node pairs is significant enough to warrant a waypoint
	bool SetPath(vector<MapSearchNode*>& vPath, dword dwResolution, float fLinearAccuracy);

	Waypoint* GetStartWaypoint();
	void InitWaypointTraversal();
	Waypoint* TraverseWaypoint();
	inline Waypoint* CurrentWaypoint() { return m_pCurrWaypoint; };
};

#endif
