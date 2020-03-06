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

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

#ifdef PROJECT_DIR_PATH
string path = string(PROJECT_DIR_PATH) + "/resource/video/o2d3y-yuv402p-512x512.yuv";
#endif

const int width = 512,height = 512;

int main() {

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("Hello SDL",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          width,height,
                                          SDL_WINDOW_ALLOW_HIGHDPI);


    FILE *pFile = fopen(path.c_str(), "rb");

    unsigned char *yuv_data;
    int frameSize = width * height * 3 / 2;
    yuv_data = static_cast<unsigned char *>(malloc(frameSize * sizeof(unsigned char)));
    fread(yuv_data,1,frameSize,pFile);

    fclose(pFile);

    SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,0);

    SDL_Texture *texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_IYUV,SDL_TEXTUREACCESS_STREAMING,width,height);


    if (texture != nullptr){
        SDL_Event windowEvent;
        while (true){
            if (SDL_PollEvent(&windowEvent)){
                if (SDL_QUIT == windowEvent.type){
                    break;
                }
            }
            SDL_UpdateTexture(texture, nullptr,yuv_data,width);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer,texture, nullptr, nullptr);
            SDL_RenderPresent(renderer);
        }
        SDL_DestroyWindow(window);
        SDL_Quit();
    }



    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}