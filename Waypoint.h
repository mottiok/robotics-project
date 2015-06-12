#ifndef WAYPOINT_H
#define WAYPOINT_H

#include "Map.h"
#include "typedefs.h"

class Waypoint
{
	SPosition m_posPosition;
	Waypoint* m_pNextWaypoint;

	public:

	Waypoint(SPosition Position, Waypoint* pNextWP = NULL) : 
		m_posPosition(Position), m_pNextWaypoint(pNextWP) {};
	
	inline Waypoint* GetNextWaypoint() { return m_pNextWaypoint; };
	inline void 	 SetNextWaypoint(Waypoint* pNextWP) { m_pNextWaypoint = pNextWP; };
	inline void 	 SetPosition(dword dwX, dword dwY) { m_posPosition.dwX = dwX; m_posPosition.dwY = dwY; };
	inline SPosition& GetPosition() { return m_posPosition; };
	inline dword	 GetXPos() { return m_posPosition.dwX; };
	inline dword 	 GetYPos() { return m_posPosition.dwY; };
};

#endif
