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
#ifndef AV_BEGINNER_PLAYER_H
#define AV_BEGINNER_PLAYER_H

#include <iostream>
#include <string>

using namespace std;

extern "C" {
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libswresample/swresample.h>
}

#include <ffmpeg_log.h>
#include <thread>


class Player {

public:

    Player() = default;

    virtual int open(string path);
    virtual int initContext();

private:

    shared_ptr<AVCodecContext> spVideoCodecCtx, spAudioCodecCtx;
    shared_ptr<AVCodec> spVideoCodec, spAudioCodec;
    shared_ptr<AVFormatContext> spFormatContext;
    shared_ptr<AVPacket> spPacket;
    shared_ptr<SwsContext> spVideoConvertCtx;
    shared_ptr<SwrContext> spAudioConvextCtx;

    int videoIndex,audioIndex;


};


#endif //AV_BEGINNER_PLAYER_H
