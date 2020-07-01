//
// Created by glumes on 2020/2/6.
//

#ifndef FFMPEG_MAC_CLION_SDLWRAPPER_H
#define FFMPEG_MAC_CLION_SDLWRAPPER_H

#include <SDL.h>

class SDLWrapper {

private:
//    SDL_Renderer *sdl_renderer;
//    SDL_Texture *sdl_texture;
//    SDL_Window *sdl_window;

public:

    int init();
    void createWindow(const char* title,int width,int height);
    void createRenderer();
    void createTexture(int width,int height);


    SDL_Renderer *sdl_renderer;
    SDL_Texture *sdl_texture;
    SDL_Window *sdl_window;

    void destroyWindow();
    void destroyTexture();
    void destroyRenderer();
    void quit();

};


#endif //FFMPEG_MAC_CLION_SDLWRAPPER_H
