/*
 * Map.cpp
 *
 *  Created on: May 29, 2015
 *      Author: colman
 */

#include "Map.h"

Map::Map(Robot* robot, const char* filename) {
	_robot = robot;
	LoadMap(filename);
}

void Map::LoadMap(const char* filename) {
	vector<unsigned char> image; // The raw pixels
	unsigned width, height;

	// Decode
	unsigned error = lodepng::decode(image, width, height, filename);

	// If there's an error, display it
	if (error)
		cout << "decoder error " << error << ": " << lodepng_error_text(error)
				<< endl;

	_map = new int[width*height];

	vector<Object> objects;
	unsigned x, y;

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (image[y * width * 4 + x * 4 + 0]
					|| image[y * width * 4 + x * 4 + 1]
					|| image[y * width * 4 + x * 4 + 2]) {
				// This is true when the color isn't black (any color)
				// Do Nothing... (For Now)
				_map[y * width + x] = 0;
			} else {
				//objects.push_back(Object(x, y));
				_map[y * width + x] = 1;
			}
		}
	}

	// Rendering using SDL2

	_window = SDL_CreateWindow("QuadTree", SDL_WINDOWPOS_UNDEFINED,
					SDL_WINDOWPOS_UNDEFINED, width, height, 0);

	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

	// Render a white background
	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
	SDL_RenderClear(_renderer);

	// Render the objects in black
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if (_map[y * width + x] != 0) {
				DrawPoint(x, y, 0, 0, 0, false);
			}
		}
	}

	SDL_RenderPresent(_renderer);
	SDL_Delay(10);
}

void Map::DrawPoint(int x, int y, Uint8 r, Uint8 g, Uint8 b, bool render) {
	SDL_SetRenderDrawColor(_renderer, r, g, b, 255);
	SDL_RenderDrawPoint(_renderer, x, y);

	if (render) {
		SDL_RenderPresent(_renderer);
		SDL_Delay(10);
	}
}

Map::~Map() {
	delete [] _map;

	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
}

