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

    virtual int init();
    virtual int open(string path,int width = -1,int height = -1);
    virtual int initVideoFrame(shared_ptr<AVFrame> &,shared_ptr<AVCodecContext> &);
    virtual int initVideoSwsContext(shared_ptr<SwsContext> &,shared_ptr<AVCodecContext> &);
    virtual int initAudioSwrContext(shared_ptr<SwrContext> &);
    virtual int openCodecContext(int *stream_idx,shared_ptr<AVCodecContext> &,shared_ptr<AVFormatContext> &,enum AVMediaType type);

protected:

    shared_ptr<AVCodecContext> spVideoCodecCtx, spAudioCodecCtx;
    shared_ptr<AVCodec> spVideoCodec, spAudioCodec;
    shared_ptr<AVFormatContext> spFormatContext;
    shared_ptr<AVPacket> spPacket;
    shared_ptr<SwsContext> spVideoConvertCtx;
    shared_ptr<SwrContext> spAudioConvertCtx;

    shared_ptr<AVFrame> spVideoFrame ;
    shared_ptr<AVFrame> spYUVFrame;

    shared_ptr<uint8_t> spBuffer;
    int videoIndex = -1,audioIndex = -1;
    int mVideoWidth = -1,mVideoHeight = -1;

    shared_ptr<SwsContext> spVideoSwsContext;

};


#endif //AV_BEGINNER_PLAYER_H
