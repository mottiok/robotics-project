/* 
 * File:   SDL2Wrapper.cpp
 * Author: colman
 * 
 * Created on June 9, 2015, 3:00 PM
 */

#include "SDL2Wrapper.h"
#include "typedefs.h"

SDL2Wrapper::SDL2Wrapper() {
    _window = NULL;
    _renderer = NULL;
}

SDL2Wrapper::~SDL2Wrapper() {
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
}

void SDL2Wrapper::CreateWindow(const char* title, int width, int height) {
    if (_window && _renderer) {
        printf("This wrapper doesn't support multiple windows creation.");
        return;
    }
    
    _window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, width, height, 0);

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
}

void SDL2Wrapper::SetBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);
    SDL_RenderClear(_renderer);
}

void SDL2Wrapper::DrawPoint(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool flushImmediately) {
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);
    SDL_RenderDrawPoint(_renderer, x, y);
    
    if (flushImmediately) {
        FlushChanges();
    }
}

void SDL2Wrapper::DrawPoint(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    DrawPoint(x, y, r, g, b, a, false);
}

void SDL2Wrapper::FillRectangle(int x, int y, int size, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool flushImmediately) {
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);
    SDL_Rect rect = {x, y, size, size};
    SDL_RenderFillRect(_renderer, &rect);
    
    if (flushImmediately) {
        FlushChanges();
    }
}

void SDL2Wrapper::FlushChanges() {
    FlushChanges(0);
}

void SDL2Wrapper::FlushChanges(int ms) {
    SDL_Event event;
    SDL_PollEvent(&event);
    SDL_RenderPresent(_renderer);
    
    if (ms > 0) {
        Delay(ms);
    }
}

void SDL2Wrapper::Delay(int ms) {
    SDL_Delay(ms);
}