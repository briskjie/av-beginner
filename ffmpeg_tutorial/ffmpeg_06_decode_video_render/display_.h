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

#ifndef AV_BEGINNER_DISPLAY__H
#define AV_BEGINNER_DISPLAY__H

#include <iostream>
#include <SDL.h>
#include <array>
#include <ffmpeg_log.h>

struct SDL {
    SDL();

    ~SDL();
};


class Display {

public:

    Display(const unsigned int width, const unsigned int height);

    void refresh(std::array<uint8_t *,3> planes,std::array<size_t,3> pitches);

    void input();

    bool get_quit();

    bool get_play();

    std::unique_ptr<SDL_Window, void (*)(SDL_Window *)> window_;
    std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)> renderer_;
    std::unique_ptr<SDL_Texture, void (*)(SDL_Texture *)> texture_;
    SDL_Event event_;

    SDL sdl_;
private:
    bool quit_{false};

    bool play_{true};
};


#endif //AV_BEGINNER_DISPLAY__H
