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

/**
 * mp4 视频文件转成 yuv 文件命令：
 * ffmpeg -i video.mp4 video.yuv
 *
 * 运行该程序前确保在 /resource/gen/ 目录下生成了对应的 yuv 文件
 * 因为文件过大，不方便 GitHub 上传，手动生成一下
 * ffmpeg -i video-mp4-640x360.mp4 video-yuv-640x360.yuv
 *
 * 播放 yuv 视频文件的命令如下：
 * ffplay -f rawvideo -video_size  100x100 yuv_filename.yuv
 */

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

#ifdef PROJECT_DIR_PATH
string path = string(PROJECT_DIR_PATH) + "/resource/gen/video-yuv-640x360.yuv";
#endif

const int width = 640,height = 360;

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

    if (!pFile){
        cout << "file not exist!!!" << endl;
        return -1;
    }

    // 打开文件
    unsigned char *yuv_data;
    int frameSize = width * height * 3 / 2;
    yuv_data = static_cast<unsigned char *>(malloc(frameSize * sizeof(unsigned char)));

    SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,0);
    // 创建纹理
    SDL_Texture *texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_IYUV,SDL_TEXTUREACCESS_STREAMING,width,height);

    if (texture != nullptr){
        SDL_Event windowEvent;
        while (true){
            if (SDL_PollEvent(&windowEvent)){
                if (SDL_QUIT == windowEvent.type){
                    break;
                }
            }
            // 读取内容
            if (fread(yuv_data,1,frameSize,pFile) != frameSize){
                // 读取内容小于 frameSize ，seek 到 0 ，重新读取，类似于重播
                fseek(pFile,0,SEEK_SET);
                fread(yuv_data,1,frameSize,pFile);
            }
            //
            SDL_UpdateTexture(texture, nullptr,yuv_data,width);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer,texture, nullptr, nullptr);
            SDL_RenderPresent(renderer);
        }
        SDL_DestroyWindow(window);
        SDL_Quit();
    }


    texture = nullptr;
    free(yuv_data);
    fclose(pFile);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}