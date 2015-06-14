/* 
 * File:   Convert.h
 * Author: colman
 *
 * Created on June 14, 2015, 4:46 AM
 */

#ifndef CONVERT_H
#define	CONVERT_H

#define CM_TO_METERS(cm) cm / 100

class Convert {
public:
	/*
	 * All pixel coordinates to relative robot positions and vice versa don't scale the given values
	 * Relative robot and resolution should be in meters
	 */
	static double PixelXCoordToRobotRelativeXPos(double dX, double resolution, double mapWidth) {
		double quarterSize = mapWidth / 2;
		
		if (dX > quarterSize)
			dX -= quarterSize;
		else {
			dX = quarterSize - dX;
			dX *= -1;			
		}
		
		return dX * resolution;
	}
	
	static double PixelYCoordToRobotRelativeYPos(double dY, double resolution, double mapHeight) {
		double quarterSize = mapHeight / 2;
		
		if (dY > quarterSize) {
			dY -= quarterSize;
			dY *= -1;
		} else {
			dY = quarterSize - dY;
		}
		
		return dY * resolution;
	}
	
	static double RobotRelativeXPosToPixelXCoord(double dX, double resolution, double mapWidth) {
		double quarterSize = (mapWidth / 2) * resolution;
		
		if (dX > 0)
			dX += quarterSize;
		else {
			dX *= -1;
			dX = quarterSize - dX;
		}
		
		return dX / resolution;
	}
	
	static double RobotRelativeYPosToPixelYCoord(double dY, double resolution, double mapHeight) {
		double quarterSize = (mapHeight / 2) * resolution;
		
		if (dY < 0) {
			dY *= -1;
			dY += quarterSize;
		} else {
			dY = quarterSize - dY;
		}
		
		return dY / resolution;
	}
};

#endif	/* CONVERT_H */

