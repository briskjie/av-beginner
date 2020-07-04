/*
* Copyright 2020 https://glumes.com
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     https://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "display_.h"

template<typename T>
inline T check_SDL(T value, const std::string &message) {
    if (!value) {
        throw std::runtime_error{"SDL " + message};
    } else {
        return value;
    }
}

SDL::SDL() {
    check_SDL(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER), "init");
    log("init sdl");
}

SDL::~SDL() {
    SDL_Quit();
}

Display::Display(const unsigned int width, const unsigned int height) :
        window_{
                check_SDL(SDL_CreateWindow("player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                           width, height,
                                           SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE), "window"),
                SDL_DestroyWindow},
        renderer_{
                check_SDL(SDL_CreateRenderer(window_.get(), -1,0),
                          "renderer"),
                SDL_DestroyRenderer},
        texture_{
                check_SDL(SDL_CreateTexture(renderer_.get(),
                                            SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,
                                            width, height), "texture"),
                SDL_DestroyTexture
        } {

    log("init display");
//    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
//    SDL_RenderSetLogicalSize(renderer_.get(), width, height);
    SDL_SetRenderDrawColor(renderer_.get(), 255, 0, 0, 255);
//    SDL_RenderClear(renderer_.get());
//    SDL_RenderPresent(renderer_.get());
}

void Display::refresh(std::array<uint8_t *, 3> planes, std::array<size_t, 3> pitches) {
    check_SDL(SDL_UpdateYUVTexture(texture_.get(), nullptr,
                                    planes[0], pitches[0],
                                    planes[1], pitches[1],
                                    planes[2], pitches[2]), "texture update");

    log("start refresh");
//    SDL_UpdateYUVTexture(texture_.get(), nullptr,
//                         planes[0], pitches[0],
//                         planes[1], pitches[1],
//                         planes[2], pitches[2]);
    SDL_RenderClear(renderer_.get());
    SDL_RenderCopy(renderer_.get(), texture_.get(), nullptr, nullptr);
    SDL_RenderPresent(renderer_.get());
    SDL_Delay(40);

}

void Display::input() {
    if (SDL_PollEvent(&event_)) {
        switch (event_.type) {
            case SDL_KEYUP:
                switch (event_.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quit_ = true;
                        break;
                    case SDLK_SPACE:
                        play_ = !play_;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_QUIT:
                quit_ = true;
                break;
            default:
                break;
        }
    }
}

bool Display::get_quit() {
    return quit_;
}

bool Display::get_play() {
    return play_;
}


