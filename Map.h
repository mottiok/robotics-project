/*
 * Map.h
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#ifndef MAP_H_
#define MAP_H_

#include "SDL2/SDL.h"
#include "Libraries/lodepng.h"
#include "Robot.h"
#include <vector>
using namespace std;
#include "Object.h"

class Map {
	Robot* _robot;
	int* _map;
	SDL_Window* _window;
	SDL_Renderer* _renderer;
public:
	Map(Robot* robot, const char* filename);
	void DrawPoint(int x, int y, Uint8 r, Uint8 g, Uint8 b, bool render);
	virtual ~Map();
private:
	void LoadMap(const char* filename);
};

#endif /* MAP_H_ */
