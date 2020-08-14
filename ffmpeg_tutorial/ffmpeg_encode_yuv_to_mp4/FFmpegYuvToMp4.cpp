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
*/#include "FFmpegYuvToMp4.h"


FFmpegYuvToMp4::FFmpegYuvToMp4():mInputFilePath(""),mOutputFilePath(""),mWidth(-1),mHeight(-1) {

}

int FFmpegYuvToMp4::initContext() {
    av_register_all();

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
    pCodecCtx->gop_size = 12;

    pCodecCtx->qmin = 10;
    pCodecCtx->qmax = 51;

    pCodecCtx->max_b_frames = 3;

//    AVDictionary  *param = 0;
//    if (pCodecCtx->codec_id == AV_CODEC_ID_H264){
//        av_dict_set(&param,"preset","slow",0);
//        av_dict_set(&param,"tune","zerolatency",0);
//    }
//
//    if (pCodecCtx->codec_id == AV_CODEC_ID_H265){
//        av_dict_set(&param,"preset","ultrafast",0);
//        av_dict_set(&param,"tune","zero-latency",0);
//    }

    av_dump_format(pFormatCtx,0,mOutputFilePath.c_str(),1);

    pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
    if (!pCodec){
        log("can not find encoder");
        return RET_FAIL;
    }
    int ret;
    if ( (ret = avcodec_open2(pCodecCtx,pCodec, nullptr) )< 0){
        log("open encoder failed and ret is %d",ret);
        return RET_FAIL;
    }

    return 0;
}

int FFmpegYuvToMp4::open(string input, string output, int width, int height) {
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

int FFmpegYuvToMp4::initBuffer() {
    pFrame = av_frame_alloc();
    picture_size = avpicture_get_size(pCodecCtx->pix_fmt,pCodecCtx->width,pCodecCtx->height);
    picture_buf = (uint8_t*)av_malloc(picture_size);
    avpicture_fill((AVPicture*)pFrame,picture_buf,pCodecCtx->pix_fmt,pCodecCtx->width,pCodecCtx->height);
    return 0;
}

int FFmpegYuvToMp4::writeHeader() {
    avformat_write_header(pFormatCtx, nullptr);
    av_new_packet(&pPacket,picture_size);
    y_size = pCodecCtx->width * pCodecCtx->height;

    return 0;
}

int FFmpegYuvToMp4::encodeData() {
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
        int got_picture = 0;
        int ret = avcodec_encode_video2(pCodecCtx,&pPacket,pFrame,&got_picture);
        if (ret < 0){
            log("faile to encode");
            return RET_FAIL;
        }
        if (got_picture == 1){
            framecnt ++;
            pPacket.stream_index = video_st->index;
            av_packet_rescale_ts(&pPacket,pCodecCtx->time_base,video_st->time_base);
            pPacket.pos = -1;
            ret = av_interleaved_write_frame(pFormatCtx,&pPacket);
            av_free_packet(&pPacket);
        }
    }

    return 0;
}

int FFmpegYuvToMp4::writeTrailer() {
    av_write_trailer(pFormatCtx);
    return 0;
}

FFmpegYuvToMp4::~FFmpegYuvToMp4() {
    if (video_st){
        avcodec_close(video_st->codec);
        av_free(pFrame);
        av_free(picture_buf);
    }

    avio_close(pFormatCtx->pb);
    avformat_free_context(pFormatCtx);
    fclose(input_file);

}
