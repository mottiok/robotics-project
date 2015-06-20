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

// Localization Manager
#define LOW_BELIF_THRESHOLD 0.2
#define HIGH_BELIF_THRESHOLD 0.7
#define PARTICLE_NORMAL_BREED 2
#define PARTICLE_HIGH_BREED 5
#define MAX_PARTICLES_COUNT 15
#define PARTICLE_EMERGENCY_BREED MAX_PARTICLES_COUNT - 2

// Particle
#define EXPANSION_RADIUS 0.3
#define EMERGENCY_EXPANSION_RADIUS 5
#define BELIF_MAGIC_NUMBER 2
#define PARTICLE_LIFES 5

// Waypoint Manager
#define WAYPOINT_RADIUS 3
#define DEFAULT_WAYPOINT_RESOLUTION 4
#define DEFAULT_WAYPOINT_ACCURACY (0.1F)

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

