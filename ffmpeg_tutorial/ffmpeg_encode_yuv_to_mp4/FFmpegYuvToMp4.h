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

#ifndef AV_BEGINNER_FFMPEGYUVTOMP4_H
#define AV_BEGINNER_FFMPEGYUVTOMP4_H


#include <iostream>
#include <string>

using namespace  std;

extern "C" {
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
}

#include <ffmpeg_log.h>
class FFmpegYuvToMp4 {

public:
    FFmpegYuvToMp4();

    ~FFmpegYuvToMp4();

    int open(string input = "",string output = "",int width = -1,int height = -1);
    int initContext();
    int initBuffer();
    int writeHeader();
    int encodeData();
    int writeTrailer();


private:
    string mInputFilePath;
    string mOutputFilePath;
    int mWidth;
    int mHeight;

    AVFormatContext *pFormatCtx;
    AVOutputFormat *pOutputFormatCtx;
    AVStream  *video_st;
    AVCodecContext *pCodecCtx;

    AVCodec* pCodec;
    FILE *input_file = nullptr;

    AVPacket pPacket;
    AVFrame * pFrame;
    int picture_size;
    int y_size;
    uint8_t *picture_buf;
    int framenum = 100;
    int framecnt = 0;
};


#endif //AV_BEGINNER_FFMPEGYUVTOMP4_H
