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

//static std::function<void(void *, int)> pgm_save = [](void *frame, int num) {
//    AVFrame *avFrame = static_cast<AVFrame *>(frame);
//    char single_frame_path[1024];
//    sprintf(single_frame_path, "%s-%d.pgm", pgm_path.c_str(), num);
//
//    FILE *f;
//    f = fopen(single_frame_path, "wb+");
//    fprintf(f, "P5\n%d %d\n%d\n", avFrame->width, avFrame->height, 255);
//
//    for (int i = 0; i < avFrame->height; ++i) {
//        fwrite(avFrame->data[0] + i * avFrame->linesize[0], 1, avFrame->width, f);
//    }
//    fclose(f);
//    log("success write %d frame to pgm file",num);
//};


static bool send(AVCodecContext *codec_context,AVPacket *packet){
    auto ret = avcodec_send_packet(codec_context,packet);
    return !(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF);
}

static bool receive(AVCodecContext *codec_context,AVFrame* frame){
    auto ret = avcodec_receive_frame(codec_context,frame);
    return !(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF);
}

static void decode_thread(Decoder *decoder){
    log("this is decode thread\n");

    int count = 0;
    for(;;){

        std::unique_ptr<AVPacket,std::function<void(AVPacket*)>> packet{
            new AVPacket,[](AVPacket*p){
                av_packet_unref(p);
                delete p;
            }
        };

        av_init_packet(packet.get());

        packet->data = nullptr;

        int ret = av_read_frame(decoder->mFormatContext,packet.get());
        if(ret < 0){
            decoder->packetQueue->finished();
            log("finish and ret is %d",ret);
            break;
        }

        log("ret is %d and stream index is %d and video index is %d",ret,packet->stream_index,decoder->mVideoStreamIdx);

        if (packet->stream_index == decoder->mVideoStreamIdx){
            count ++;
        }

        if (packet->stream_index == decoder->mVideoStreamIdx){
            if(!decoder->packetQueue->push(move(packet))){
                log("push packet");
                break;
            }
        }

        log("video count is %d",count);
    }

    log("finish decode thread");
}

static void read_thread(Decoder *decoder){
    log("this is read thread\n");

    std::function<void(void*,int)> save = yuv_save;

    for(;;){
        std::unique_ptr<AVFrame,std::function<void(AVFrame*)>> frame_decoded{
            av_frame_alloc(),[](AVFrame *f){av_frame_free(&f);}
        };

        std::unique_ptr<AVPacket,std::function<void(AVPacket *)>> packet {
                nullptr,[](AVPacket *p){av_packet_unref(p);delete p;}
        };

        if(!decoder->packetQueue->pop(packet)){
            break;
        }

        bool sent = false;
        while (!sent){

            sent = send(decoder->mVideoDecContext,packet.get());

            while (receive(decoder->mVideoDecContext,frame_decoded.get())){
                save(frame_decoded.get(),decoder->mVideoDecContext->frame_number);
            }
        }
    }
    log("finish read thread");
}

Decoder::Decoder(string path) : mPath(path) {
    packetQueue = make_unique<PacketQueue>(2000);
}


Decoder::~Decoder() {

}

int Decoder::openInputFile() {
    return openInputFile(mFormatContext, mPath.c_str());
}

int Decoder::openCodec() {
    openCodecContext(&mVideoStreamIdx, &mVideoDecContext, mFormatContext, AVMEDIA_TYPE_VIDEO);
    openCodecContext(&mAudioStreamIdx, &mAudioDecContext, mFormatContext, AVMEDIA_TYPE_AUDIO);
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
    int ret ;
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
        *stream_idx = stream_index;
    }
    return ret;
}

int Decoder::decodeVideo() {

    videoPacket = av_packet_alloc();
    videoFrame = av_frame_alloc();

    startDecodeVideo(&mFormatContext, &mVideoDecContext, &videoPacket,&videoFrame,mVideoStreamIdx);

    av_frame_free(&videoFrame);
    av_packet_free(&videoPacket);
    return 0;
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

int Decoder::startDecodeVideo(AVFormatContext **fmt_ctx, AVCodecContext **codec_ctx, AVPacket **packet,AVFrame **frame,int videoIndex) {

    while (av_read_frame(*fmt_ctx, *packet) == 0) {
        if ((*packet)->stream_index == videoIndex){
            decode(mVideoDecContext,*packet,*frame);
        }
        // 没有这一行会有 Invalid NAL unit size (65536 > 60) 错误
        av_packet_unref(*packet);
    }
    decode(mVideoDecContext,*packet,*frame);
    return 0;
}

int Decoder::decode(AVCodecContext *codec_ctx, AVPacket *packet, AVFrame *frame,std::function<void(void*,int)> func) {
    int ret = avcodec_send_packet(codec_ctx,packet);

    if (ret < 0){
        logE("error send packet for decoding and ret is %d\n",ret);
        return RET_FAIL;
    }

    while (ret >= 0){
        ret = avcodec_receive_frame(codec_ctx,frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF){
            return RET_OK;
        } else if (ret < 0){
            logE("error during decoding and ret is %d\n",ret);
            return RET_FAIL;
        }
        if (func){
            func(frame,codec_ctx->frame_number);
        }
    }
    return 0;
}


int Decoder::startDecodeThread() {

    thread decodeThread(decode_thread,this);

    decodeThread.join();

    log("finish decode");

    thread readThread(read_thread,this);

    readThread.join();

    log("finish read");

    return 0;
}
