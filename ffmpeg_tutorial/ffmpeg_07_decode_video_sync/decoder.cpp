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


static bool send(AVCodecContext *codec_context, AVPacket *packet) {
    auto ret = avcodec_send_packet(codec_context, packet);
    return !(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF);
}

static bool receive(AVCodecContext *codec_context, AVFrame *frame) {
    auto ret = avcodec_receive_frame(codec_context, frame);
    return !(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF);
}

static void demux_thread(Decoder *decoder) {
    log("this is decode thread\n");

    int count = 0;
    for (;;) {

        // move 操作并不会转移删除器
        std::unique_ptr<AVPacket, std::function<void(AVPacket *)>> packet{
                new AVPacket, [](AVPacket *p) {
                    av_packet_unref(p);
                    av_packet_free(&p);
//                    log("delete packet in decode thread");
                }
        };

        av_init_packet(packet.get());

        packet->data = nullptr;

        int ret = av_read_frame(decoder->mFormatContext, packet.get());
        if (ret < 0) {
            decoder->packetQueue->finished();
            log("finish and ret is %d", ret);
            break;
        }

//        log("ret is %d and stream index is %d and video index is %d", ret, packet->stream_index,
//            decoder->mVideoStreamIdx);

        if (packet->stream_index == decoder->mVideoStreamIdx) {
            count++;
            if (!decoder->packetQueue->push(move(packet))) {
                log("push packet and queue size is %d", decoder->packetQueue->count());
                break;
            }
        }

        log("video count is %d", count);
    }

    log("finish decode thread");
}

static void decode_video_thread(Decoder *decoder) {
    log("this is read thread\n");

    std::function<void(void *, int)> save = pgm_save;

    for (;;) {
        std::unique_ptr<AVFrame, std::function<void(AVFrame *)>> frame_decoded{
                av_frame_alloc(), [](AVFrame *f) {
                    av_frame_unref(f);
                    av_frame_free(&f);
                    log("delete frame in read thread");
                }
        };

        std::unique_ptr<AVPacket, std::function<void(AVPacket *)>> packet{
                nullptr, [](AVPacket *p) {
                    av_packet_unref(p);
                    av_packet_free(&p);
                    log("delete packet in read thread");
                }
        };

        log("pop packet");
        if (!decoder->packetQueue->pop(packet)) {
            log("quit decode thread");
            break;
        }

        bool sent = false;
        while (!sent) {

            sent = send(decoder->mVideoDecContext, packet.get());

            while (receive(decoder->mVideoDecContext, frame_decoded.get())) {
//                save(frame_decoded.get(), decoder->mVideoDecContext->frame_number);
                decoder->frameQueue->push(move(frame_decoded));
                break;
            }
        }
    }
    log("finish read thread");
}

Decoder::Decoder(string path) : mPath(path) {
    packetQueue = make_unique<PacketQueue>(10);
    frameQueue = make_unique<FrameQueue>(10);
}

Decoder::~Decoder() {
    log("~Decoder queue size is %d", packetQueue->count());
    if (mFormatContext != nullptr) {
        avformat_free_context(mFormatContext);
        log("free format context");
    }

    if (mVideoDecContext != nullptr) {
        avcodec_free_context(&mVideoDecContext);
        log("free video context");
    }

    if (mAudioDecContext != nullptr) {
        avcodec_free_context(&mAudioDecContext);
        log("free audio context");
    }

}

int Decoder::openInputFile() {
    return openInputFile(mFormatContext, mPath.c_str());
}

int Decoder::openCodec() {
    openCodecContext(&mVideoStreamIdx, &mVideoDecContext, mFormatContext, AVMEDIA_TYPE_VIDEO);
//    openCodecContext(&mAudioStreamIdx, &mAudioDecContext, mFormatContext, AVMEDIA_TYPE_AUDIO);
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
Decoder::openCodecContext(int *stream_idx, AVCodecContext **codec_ctx,
                          AVFormatContext *fmt_ctx, enum AVMediaType type) {
    int ret;
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

        *codec_ctx = avcodec_alloc_context3(codec);
        if (!*codec_ctx) {
            logE("failed to allocate the %s codec context", av_get_media_type_string(type));
            return AVERROR(ENOMEM);
        }

        if ((ret = avcodec_parameters_to_context(*codec_ctx, stream->codecpar)) < 0) {
            logE("failed to copy %s codec parameters to decoder context", av_get_media_type_string(type));
            return ret;
        }

        if ((ret = avcodec_open2(*codec_ctx, codec, nullptr)) < 0) {
            logE("failed to open %s codec", av_get_media_type_string(type));
            return ret;
        }
        mWidth = (*codec_ctx)->width;
        mHeight = (*codec_ctx)->height;
        log("widht is %d and height is %d", mWidth, mHeight);
        *stream_idx = stream_index;
    }
    return ret;
}


void Decoder::dumpInfo() {
    logI("file path is %s", mFormatContext->filename);
    logI("iformat name is %s", mFormatContext->iformat->name);
    logI("nb_streams is %d", mFormatContext->nb_streams);
    logI("bitrate is %lld", mFormatContext->bit_rate);
    logI("duration is %lld", mFormatContext->duration);

    logI("width is %d", mVideoDecContext->width);
    logI("height is %d", mVideoDecContext->height);
}


void Decoder::startDemultiplex() {

    // 解码线程
    std::thread demuxThread(demux_thread, this);

//    demuxThread.join();
//    mDemuxThread = thread(demuxThread,this);
//    return 0;
}

void Decoder::startDecodeVideo(){

    thread decodeVideoThread(decode_video_thread,this);

}

