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

#include "decoder.h"

Decoder::Decoder(string path) : mPath(path) {

}


Decoder::~Decoder() {

}

int Decoder::openInputFile() {
    return openInputFile(mFormatContext, mPath.c_str());
}

int Decoder::openCodec() {
    openCodecContext(mVideoStreamIdx, mVideoDecContext, mFormatContext, AVMEDIA_TYPE_VIDEO);
    openCodecContext(mAudioStreamIdx, mAudioDecContext, mFormatContext, AVMEDIA_TYPE_AUDIO);
    return 0;
}

int Decoder::openInputFile(AVFormatContext *&fmt_ctx, const char *filename) {
    fmt_ctx = avformat_alloc_context();

    int ret = RET_FAIL;
    if ((ret = avformat_open_input(&fmt_ctx, filename, nullptr, nullptr)) < 0) {
        logE("Cannot open input file");
        return ret;
    }

    if ((ret = avformat_find_stream_info(fmt_ctx, nullptr)) < 0) {
        logE("Cannot find stream information");
        return ret;
    }

    logI("open input file success");
    return ret;
}

int
Decoder::openCodecContext(int &stream_idx, AVCodecContext *&codec_ctx,
                         AVFormatContext *fmt_ctx, enum AVMediaType type) {
    int ret = RET_FAIL;
    int stream_index;
    AVStream *stream;
    AVCodec *codec;
    ret = av_find_best_stream(fmt_ctx, type, -1, -1, nullptr, 0);

    if (ret < 0) {
        logE("could not find %s stream in input file", av_get_media_type_string(type));
        return ret;
    } else {
        stream_index = ret;
        stream = fmt_ctx->streams[stream_index];

        codec = avcodec_find_decoder(stream->codecpar->codec_id);
        if (!codec) {
            logE("could not find %s codec in input file", av_get_media_type_string(type));
            return AVERROR(EINVAL);
        }

        codec_ctx = avcodec_alloc_context3(codec);
        if (!codec_ctx) {
            logE("failed to allocate the %s codec context", av_get_media_type_string(type));
            return AVERROR(ENOMEM);
        }

        if ((ret = avcodec_parameters_to_context(codec_ctx, stream->codecpar)) < 0) {
            logE("failed to copy %s codec parameters to decoder context", av_get_media_type_string(type));
            return ret;
        }

        if ((ret = avcodec_open2(codec_ctx, codec, nullptr)) < 0) {
            logE("failed to open %s codec", av_get_media_type_string(type));
            return ret;
        }
        stream_idx = stream_index;
    }
    return ret;
}

void Decoder::dumpInfo() {
    logI("file path is %s", mFormatContext->filename);
    logI("iformat name is %s", mFormatContext->iformat->name);
    logI("nb_streams is %d", mFormatContext->nb_streams);
    logI("bitrate is %lld", mFormatContext->bit_rate);
    logI("duration is %lld", mFormatContext->duration);

    logI("width is %d",mVideoDecContext->width);
    logI("height is %d",mVideoDecContext->height);
}

int Decoder::decodeVideo() {
    return 0;
}
