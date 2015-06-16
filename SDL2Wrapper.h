/* 
 * File:   SDL2Wrapper.h
 * Author: colman
 *
 * Created on June 9, 2015, 3:00 PM
 */

#ifndef SDL2WRAPPER_H
#define	SDL2WRAPPER_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

class SDL2Wrapper {
    SDL_Window* _window;
    SDL_Renderer* _renderer;
	SDL_Texture* _texture;
public:
    SDL2Wrapper();
    virtual ~SDL2Wrapper();
    
    void CreateWindow(const char* title, int width, int height);
    void SetBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void DrawPoint(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void DrawPoint(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool flushImmediately);
    void FillRectangle(int x, int y, int size, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool flushImmediately);
    void FlushChanges();
    void FlushChanges(int ms);
    void Delay(int ms);
	void LoadBackground(const char* filename, bool flushImmediately);
private:

};

#endif	/* SDL2WRAPPER_H */

