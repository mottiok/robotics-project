/* 
 * File:   Convert.h
 * Author: colman
 *
 * Created on June 14, 2015, 4:46 AM
 */

#ifndef CONFIGURATION_H
#define	CONFIGURATION_H

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include "typedefs.h"

using namespace std;

// Main
#define PLAYER_HOST "10.10.245.64"
#define PLAYER_PORT 6665

// // Localization Manager & Particles
#define LOW_BELIF_THRESHOLD 0.2 // minimum for low belif particles
#define HIGH_BELIF_THRESHOLD 0.7 // minimum for high belif particles
#define PARTICLE_NORMAL_BREED 5 // particles child breed count for particles with normal belif
#define PARTICLE_HIGH_BREED 20 // particles child breed count for particles with high belif
#define MAX_PARTICLES_COUNT 200 // max particles count
#define EXPANSION_RADIUS 0.35 // expansion radius for each child particle
#define YAW_RANGE 0.2 // yaw range for each child particle
#define BELIF_MAGIC_NUMBER 2 // belif magic number
#define PARTICLE_LIFES 5 // particles lifes before deleted from vector
#define EMERGENCY_EXPANSION_RADIUS EXPANSION_RADIUS * 2 // emergency expansion radius when no particles exists
#define EMERGENCY_YAW_RANGE YAW_RANGE * 2 // emergency yaw range when no particles exists
#define PARTICLE_EMERGENCY_BREED MAX_PARTICLES_COUNT - 2 // Particles emergency breed count when no particles exists

// Waypoint Manager
#define WAYPOINT_RADIUS 2
#define DEFAULT_WAYPOINT_RESOLUTION 6
#define DEFAULT_WAYPOINT_ACCURACY (0.1F)
#define DETECT_EVASION_RADIUS 0.85

class Configuration {
	
	string _mapFilePath;
	dword _startX;
	dword _startY;
	dword _startYaw;
	dword _goalX;
	dword _goalY;
	double _robotSize;
	double _mapResolution;
	double _gridResolution;
	
public:
	
	const char* GetMapFilePath() { return _mapFilePath.c_str(); }
	dword GetStartX() { return _startX; }
	dword GetStartY() { return _startY; }
	dword GetStartYaw() { return _startYaw; }
	dword GetGoalX() { return _goalX; }
	dword GetGoalY() { return _goalY; }
	double GetRobotSize() { return _robotSize; }
	double GetMapResolution() { return _mapResolution; }
	double GetGridResolution() { return _gridResolution; }
	
	void LoadConfigFile(const char* filename) {
		ifstream file(filename);
		string str;
		
		for (int i=0; getline(file, str); i++) {
			string temp;
			vector<string> words;
			stringstream s(str);
			while (s >> temp)
				words.push_back(temp);
			switch (i) {
				case 0: // map file name
					_mapFilePath = words[1];
					break;
				case 1: // start coordinates
					_startX = atof(words[1].c_str());
					_startY = atof(words[2].c_str());
					_startYaw = atof(words[3].c_str());
					break;
				case 2: // goal coordinates
					_goalX = atof(words[1].c_str());
					_goalY = atof(words[2].c_str());
					break;
				case 3: // robot size
					_robotSize = atof(words[1].c_str());
					break;
				case 4: // map resolution
					_mapResolution = atof(words[1].c_str());
					break;
				case 5: // grid resolution
					_gridResolution = atof(words[1].c_str());
					break;
			}
		}
		
		file.close();
	}
};

#endif	/* CONFIGURATION_H */

