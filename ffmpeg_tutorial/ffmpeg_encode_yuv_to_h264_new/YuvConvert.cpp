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

#include "YuvConvert.h"

static bool send(AVCodecContext *codec_context, AVPacket *packet) {
    auto ret = avcodec_send_packet(codec_context, packet);
    return !(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF);
}

static bool receive(AVCodecContext *codec_context, AVFrame *frame) {
    auto ret = avcodec_receive_frame(codec_context, frame);
    return !(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF);
}

int YUVConvert::open(string input, string output, int width, int height) {
    if (width == -1 || height == -1 || input.empty() || output.empty()){
        return RET_FAIL;
    }
    mInputFilePath = input;
    mOutputFilePath = output;
    mWidth = width;
    mHeight = height;

    input_file = fopen(mInputFilePath.c_str(),"rb+");
    if (input_file == nullptr){
        log("open file failed");
    }

    log("fopen code is %d and path is %s",errno,mInputFilePath.c_str());

    return RET_OK;
}

int YUVConvert::initContext() {

    pFormatCtx = avformat_alloc_context();
    pOutputFormatCtx = av_guess_format(nullptr,mOutputFilePath.c_str(), nullptr);
    pFormatCtx->oformat = pOutputFormatCtx;

    if (avio_open(&pFormatCtx->pb,mOutputFilePath.c_str(),AVIO_FLAG_READ_WRITE) < 0){
        log("avio_open failed");
        return RET_FAIL;
    }

    video_st = avformat_new_stream(pFormatCtx,0);
    video_st->time_base.num = 1;
    video_st->time_base.den = 25;

    if (video_st == nullptr){
        return RET_FAIL;
    }


    pCodecCtx = video_st->codec;
    pCodecCtx->codec_id = pOutputFormatCtx->video_codec;
    pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    pCodecCtx->width = mWidth;
    pCodecCtx->height = mHeight;
    pCodecCtx->time_base.num = 1;
    pCodecCtx->time_base.den = 25;
    pCodecCtx->bit_rate = 400000;
    pCodecCtx->gop_size = 250;

    pCodecCtx->qmin = 10;
    pCodecCtx->qmax = 51;

    pCodecCtx->max_b_frames = 3;

//
//    video_st->codecpar->codec_id = pOutputFormatCtx->video_codec;
//    video_st->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
//    video_st->codecpar->width = mWidth;
//    video_st->codecpar->height = mHeight;
//    video_st->codecpar->bit_rate = 400000;
//
//    avcodec_parameters_to_context(pCodecCtx,video_st->codecpar);

    pCodecCtx->time_base.den = 25;
    pCodecCtx->gop_size = 250;
    pCodecCtx->time_base.num = 1;
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    pCodecCtx->qmin = 10;
    pCodecCtx->qmax = 51;
    // 在没有 B 帧的情况下，编码前的 pts 等于编码后的 pts
    // 在有 B 帧的情况下，编码前的 pts 不一定等于编码后的 pts
    pCodecCtx->max_b_frames = 0;


    AVDictionary  *param = 0;
    if (pCodecCtx->codec_id == AV_CODEC_ID_H264){
        av_dict_set(&param,"preset","slow",0);
        av_dict_set(&param,"tune","zerolatency",0);
    }

    if (pCodecCtx->codec_id == AV_CODEC_ID_H265){
        av_dict_set(&param,"preset","ultrafast",0);
        av_dict_set(&param,"tune","zero-latency",0);
    }

    av_dump_format(pFormatCtx,0,mOutputFilePath.c_str(),1);

    pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
    if (!pCodec){
        log("can not find encoder");
        return RET_FAIL;
    }
    if (avcodec_open2(pCodecCtx,pCodec,&param) < 0){
        log("open encoder failed");
        return RET_FAIL;
    }

    return 0;
}

int YUVConvert::initBuffer() {
    pFrame = av_frame_alloc();
//    picture_size = avpicture_get_size(pCodecCtx->pix_fmt,pCodecCtx->width,pCodecCtx->height);
    picture_size = av_image_get_buffer_size(pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, 1);

//    picture_buf = (uint8_t*)av_malloc(picture_size);

    picture_buf = (uint8_t *) av_malloc(
            av_image_get_buffer_size(pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, 1)
    );

    av_image_fill_arrays(pFrame->data,pFrame->linesize,picture_buf,pCodecCtx->pix_fmt,pCodecCtx->width,pCodecCtx->height,1);

//    avpicture_fill((AVPicture*)pFrame,picture_buf,pCodecCtx->pix_fmt,pCodecCtx->width,pCodecCtx->height);
    return 0;
}

int YUVConvert::writeHeader() {
    avformat_write_header(pFormatCtx, nullptr);
    av_new_packet(&pPacket,picture_size);
    y_size = pCodecCtx->width * pCodecCtx->height;

    return RET_OK;
}

int YUVConvert::encodeData() {
    for (int i = 0; i < framenum; ++i) {
        if (fread(picture_buf,1,y_size *3/2,input_file) <= 0){
            log("read file failed");
            log("error code is %d",errno);
            return -1;
        }else if (feof(input_file)){
            log("end of file");
            break;
        }
        pFrame->data[0] = picture_buf;
        pFrame->data[1] = picture_buf+y_size;
        pFrame->data[2] = picture_buf+y_size*5/4;
        pFrame->pts = i;

        int ret = avcodec_send_frame(pCodecCtx,pFrame);
//        if (ret < 0){
//            log("failed to encode");
//            continue;
//        }

        ret = avcodec_receive_packet(pCodecCtx, &pPacket);

        if (ret == 0){
            framecnt ++;
            pPacket.stream_index = video_st->index;
            ret = av_write_frame(pFormatCtx,&pPacket);

            int gap = pPacket.pts - pFrame->pts;
            log("pts gap is %d",gap);

            av_free_packet(&pPacket);
        }


    }

    return 0;
}

int YUVConvert::writeTrailer() {
    av_write_trailer(pFormatCtx);
    return 0;
}


YUVConvert::~YUVConvert() {
    log("call ~YUVConvert()  method");
}
