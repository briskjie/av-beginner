//
// Created by glumes on 2020/2/6.
//

#include "SDLWrapper.h"

int SDLWrapper::init() {
    return SDL_Init(SDL_INIT_EVERYTHING);
}

void SDLWrapper::createWindow(const char *title, int width, int height) {
    sdl_window = SDL_CreateWindow(title,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,width,height,SDL_WINDOW_ALLOW_HIGHDPI);
}

void SDLWrapper::createRenderer() {
    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, 0);
}

void SDLWrapper::createTexture(int width, int height) {
    // FFmpeg 中的像素格式 AV_PIX_FMT_YUV420P  对应于 SDL 中的 SDL_PIXELFORMAT_IYUV
    sdl_texture = SDL_CreateTexture(sdl_renderer,SDL_PIXELFORMAT_IYUV,SDL_TEXTUREACCESS_STREAMING,width,height);
}

void SDLWrapper::destroyWindow() {
    SDL_DestroyWindow(sdl_window);
}

void SDLWrapper::destroyTexture() {
    SDL_DestroyTexture(sdl_texture);
}

void SDLWrapper::destroyRenderer() {
    SDL_DestroyRenderer(sdl_renderer);
}

void SDLWrapper::quit() {
    SDL_Quit();
}

void SDLWrapper::refresh(std::array<uint8_t *, 3> planes, std::array<size_t, 3> pitches) {

    SDL_UpdateYUVTexture(sdl_texture, nullptr,
                         planes[0], pitches[0],
                         planes[1], pitches[1],
                         planes[2], pitches[2]);

    SDL_RenderClear(sdl_renderer);
    SDL_RenderCopy(sdl_renderer, sdl_texture, nullptr, nullptr);
    SDL_RenderPresent(sdl_renderer);
}

