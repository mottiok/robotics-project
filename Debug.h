#ifndef DEBUG_H
#define DEBUG_H

// Enable Drawing Behaviors
//#define DRAW_ASTAR_EXPANSION // Don't use it in production - add few second to the overall algorithm time
#define DRAW_PATH
#define DRAW_WAYPOINTS
#define DRAW_PARTICLES
#define DRAW_BEST_LASER_SCAN

// Colors
#define ASTAR_EXPANSION_COLOR 200, 255, 255
#define RED_RGB_FORMAT 255, 0, 0
#define ORANGE_RGB_FORMAT 255, 162, 0
#define GREEN_RGB_FORMAT 0, 255, 0
#define DARK_GREEN_RGB_FORMAT 0, 123, 20
#define BLUE_RGB_FORMAT 0, 0, 255
#define YELLOW_RGB_FORMAT 255, 255, 0
#define PURPLE_RGB_FORMAT 156, 22, 255
#define LIGHT_PURPLE_RGB_FORMAT 218, 165, 255

#endif
