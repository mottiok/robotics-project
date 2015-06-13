#include "WaypointManager.h"
#include <cmath>

WaypointManager::~WaypointManager()
{
	for (dword i = 0; i < m_vWaypoints.size(); i++)
	{
		delete m_vWaypoints[i];
	}
}

dword WaypointManager::FindNextSignificantNode(vector<MapSearchNode*>& vPath, dword dwStartNode, dword dwLimitNode, dword dwResolution, float fTestGradient, float fAccuracy)
{
	MapSearchNode* pTestNode = vPath[dwStartNode];
	dword dwCurrNode;
	float fCurrentGradient;

	// Begin traversing path nodes according to sample resolution
	for (dwCurrNode = dwStartNode + dwResolution; dwCurrNode < vPath.size() && dwCurrNode < dwLimitNode; dwCurrNode += dwResolution)
	{
		// Calculate current gradient 
		fCurrentGradient = SPosition::CalcGradient(pTestNode->GetPosition(), vPath[dwCurrNode]->GetPosition());

		/*printf("Comparing nodes %u (%u, %u) and %u (%u, %u), is gradient %f significant enough next to %f? ",
			dwStartNode, pTestNode->GetXPos(), pTestNode->GetYPos(), 
			dwCurrNode, vPath[dwCurrNode]->GetXPos(), vPath[dwCurrNode]->GetYPos(),
			fCurrentGradient, fTestGradient);*/

		// If gradient of line between last node and current changed enough
		if (fCurrentGradient != fTestGradient && abs(fCurrentGradient - fTestGradient) >= fAccuracy)
		{
		//	printf("yes\n");
			break;
		}

		//printf("no\n");
	}

	//printf("Next sig node from %u is %u\n", dwStartNode, dwCurrNode);

	return dwCurrNode;
}

Waypoint* WaypointManager::LinkNextWaypoint(MapSearchNode* pNode, Waypoint* pLast)
{
	Waypoint* pNewWaypoint = new Waypoint(pNode->GetPosition());
	m_vWaypoints.push_back(pNewWaypoint);

	if (NULL != pLast)
	{
		pLast->SetNextWaypoint(pNewWaypoint);
	}

	return pNewWaypoint;
}

bool WaypointManager::SetPath(vector<MapSearchNode*>& vPath, dword dwResolution, float fLinearAccuracy)
{
	if (1 >= vPath.size() || 0 == dwResolution || 0 == fLinearAccuracy)
		return false;

	Waypoint *pNewWaypoint, *pLastWaypoint;
	float fLastGradient = 0, fCurrentGradient = 0, fCurrentSubGradient = 0;

	// Starting node is always a new waypoint...
	pLastWaypoint = LinkNextWaypoint(vPath[0], NULL);

	// Initial lookup values
	fLastGradient = SPosition::CalcGradient(pLastWaypoint->GetPosition(), vPath[1]->GetPosition());

	// Begin searching for significant waypoints
	for (dword dwLastNode = 1, dwCurrNode = FindNextSignificantNode(vPath, 0, vPath.size(), dwResolution, fLastGradient, fLinearAccuracy); 
		   dwCurrNode < vPath.size(); 
		   dwCurrNode = FindNextSignificantNode(vPath, dwCurrNode, vPath.size(), dwResolution, fLastGradient, fLinearAccuracy))
	{
		fCurrentGradient = SPosition::CalcGradient(vPath[dwCurrNode - dwResolution]->GetPosition(), vPath[dwCurrNode]->GetPosition());
		
		// Found a node where there's a difference, mark as waypoint
		pLastWaypoint = LinkNextWaypoint(vPath[dwCurrNode], pLastWaypoint);

		//printf("Setting new test gradient as that between %u and %u (%f)\n", dwCurrNode - dwResolution, dwCurrNode, fCurrentGradient);
		
		dwLastNode = dwCurrNode;
		fLastGradient = fCurrentGradient;

		/*
		fCurrentSubGradient = fLastGradient;
	
		//printf("Locating all significant points in the section [%u-%u]\n", dwLastNode, dwCurrNode);

		// Begin traversing all nodes between last node and current to find all significant ones
		for (dword dwCurrSubNode = FindNextSignificantNode(vPath, dwLastNode, dwCurrNode, 1, fCurrentSubGradient, fLinearAccuracy); 
			dwCurrSubNode < dwCurrNode; 
			dwCurrSubNode = FindNextSignificantNode(vPath, dwCurrSubNode, dwCurrNode, 1, fCurrentSubGradient, fLinearAccuracy))
		{
			// Found a node where there's a difference, mark as waypoint
			pLastWaypoint = LinkNextWaypoint(vPath[dwCurrSubNode], pLastWaypoint);

			// Update next subsection gradient to look up
			fCurrentSubGradient = SPosition::CalcGradient(vPath[dwCurrSubNode-1]->GetPosition(), vPath[dwCurrSubNode]->GetPosition());
		}*/
	}

	// Set the last node as a waypoint too, of course
	LinkNextWaypoint(vPath[vPath.size()-1], pLastWaypoint);

	return true;
}

Waypoint* WaypointManager::GetStartWaypoint()
{
	Waypoint* pStart = NULL;

	if (0 != m_vWaypoints.size())
	{
		pStart = m_vWaypoints[0];
	}

	return pStart;
}

void WaypointManager::InitWaypointTraversal()
{
	m_pCurrWaypoint = GetStartWaypoint();
}

Waypoint* WaypointManager::TraverseWaypoint()
{
	Waypoint* pNextWaypoint = NULL;

	if (NULL != m_pCurrWaypoint)
	{
		pNextWaypoint = m_pCurrWaypoint->GetNextWaypoint();
		m_pCurrWaypoint = m_pCurrWaypoint->GetNextWaypoint();
	}

	return pNextWaypoint;
}
