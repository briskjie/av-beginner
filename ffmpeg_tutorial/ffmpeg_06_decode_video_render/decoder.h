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

#ifndef AV_BEGINNER_DECODER_H
#define AV_BEGINNER_DECODER_H

extern "C" {
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
}

#include <iostream>
#include "display_.h"
#include <ffmpeg_log.h>
#include "queue.h"
#include <thread>
#include <memory>
#include "avframe_write.h"

using namespace std;

class Decoder {

public:
    Decoder(string path);

    ~Decoder();


    int openInputFile();

    int openCodec();

    void dumpInfo();

    int startDecodeThread();

    AVFormatContext *mFormatContext = nullptr;
    AVCodecContext *mVideoDecContext = nullptr, *mAudioDecContext = nullptr;
    string mPath;

    unique_ptr<PacketQueue> packetQueue;

    int mVideoStreamIdx = -1;
    int mAudioStreamIdx = -1;

    Display *display;

    int mWidth = 0;
    int mHeight = 0;

    SwsContext *swsContext;

private:

    int openInputFile(AVFormatContext *&fmt_ctx, const char *filename);

    int openCodecContext(int *stream_idx, AVCodecContext **codec_ctx,
                         AVFormatContext *fmt_ctx, enum AVMediaType type);

};


#endif //AV_BEGINNER_DECODER_H
