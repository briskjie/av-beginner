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

using namespace std;

#ifdef PROJECT_DIR_PATH
string path = string(PROJECT_DIR_PATH) + "/resource/audio/music.pcm";
#endif

const int width = 500, height = 500;
static Uint8 *audio_chunk;
static Uint32 audio_len;
static Uint8 *audio_pos;

#define SDL_EVENT_BUFFER_END  (SDL_USEREVENT + 1)

void notifyGetAudioFrame(){
    SDL_Event sdlEvent;
    sdlEvent.type = SDL_EVENT_BUFFER_END;
    SDL_PushEvent(&sdlEvent);
}

void fill_audio(void *udata, Uint8 *stream, int len) {
    SDL_memset(stream, 0, len);
    if (audio_len == 0) {
        return;
    }
    // 数据大小不能超过 len
    len = len > audio_len ? audio_len : len;
    // 将 stream 和 audio_pos 进行混合播放
//    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);

    // 单独播放 audio_pos,也就是解码出来的音频数据
    memcpy(stream, audio_pos, len);

    audio_pos += len;
    audio_len -= len;

    if (audio_len <= 0){
        notifyGetAudioFrame();
    }
}


int main() {

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return -1;
    }

    FILE *pFile = fopen(path.c_str(), "rb");

    if (!pFile) {
        cout << "file not exist!!!" << endl;
        return -1;
    }

    SDL_AudioSpec audioSpec;
    audioSpec.freq = 44100;
    audioSpec.format = AUDIO_S16SYS;
    audioSpec.channels = 2;
    audioSpec.silence = 0;
    audioSpec.samples = 1024;
    audioSpec.callback = fill_audio;

    SDL_AudioDeviceID deviceId;
    if ((deviceId = SDL_OpenAudioDevice(nullptr, 0, &audioSpec, nullptr, SDL_AUDIO_ALLOW_ANY_CHANGE)) < 2) {
        cout << "open audio device failed " << endl;
        return -1;
    }

    int bufferSize = 4096;
    char *buffer = (char *) malloc(bufferSize);

    SDL_Window *window = SDL_CreateWindow("Hello SDL",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          width, height,
                                          SDL_WINDOW_ALLOW_HIGHDPI);

    SDL_Renderer *pRenderer = NULL;
    pRenderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(pRenderer, 0, 255, 0, 255);
    SDL_RenderClear(pRenderer);
    SDL_RenderPresent(pRenderer);


    int data_count = 0;

    SDL_PauseAudioDevice(deviceId, 0);

    notifyGetAudioFrame();

    SDL_Event windowEvent;
    bool bQuit = false;
    while (!bQuit) {

        while (SDL_PollEvent(&windowEvent)) {
            switch (windowEvent.type) {
                case SDL_QUIT:
                    bQuit = true;
                    cout << "receive quit \n" << endl;
                    break;
                case SDL_EVENT_BUFFER_END:
                    if (fread(buffer, 1, bufferSize, pFile)) {
                        data_count += bufferSize;
                        audio_chunk = reinterpret_cast<Uint8 *>(buffer);

                        audio_len = bufferSize;
                        audio_pos = audio_chunk;
                    }
                default:
                    break;
            }
        }
    }

    free(buffer);
    SDL_DestroyWindow(window);
    SDL_CloseAudio();
    SDL_Quit();

    return 0;
}