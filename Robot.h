/*
 * Robot.h
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include <libplayerc++/playerc++.h>
using namespace PlayerCc;

#include "LocalizationManager.h"
#include "Convert.h"

class Robot {
	PlayerClient* _pc;
	Position2dProxy* _pp;
	LaserProxy* _lp;
	LocalizationManager* _localization;
	
	double _dX;
	double _dY;
	double _dYaw;
	
	void UpdateLocalization();
	void Read();
	double SetPosition(double dX, double dY, double dYaw, bool setOdemetry);
	void SetOdemetry(double dX, double dY, double dYaw);
	
public:
	Robot(const char* ip, int port, LocalizationManager* localization);
	virtual ~Robot();
	
	void ReadAndUpdateLocalization();
	
	/*
	 * Set robot relative odometry by pixel coordinates and resolution in CM
	 * The robot's odemetry in meters and relative to X and Y axles that positioned in the center of the map
	 * and not top left corner so convert methods needed to calculate proper positions
	*/ 
	void SetOdometryByPixelCoord(double dX, double dY, double dYaw, double resolution, double mapWidth, double mapHeight);
	
	/*
	 * All Accessors return relative robot position without any conversions so be caution
	 * To convert to pixel coordinates use:
	 *		Convert::RobotRelativeXPosToPixelXCoord
	 *		Convert::RobotRelativeYPosToPixelYCoord
	 */
	double GetX();
	double GetY();
	double GetYaw();
	
	void SetSpeed(double aXSpeed, double aYawSpeed);

	bool IsForwardFree(float fRadius = 0.6F);
	bool IsRightFree(float fRadius = 0.35F);
	bool IsLeftFree(float fRadius = 0.35F);
};

#endif /* ROBOT_H_ */
