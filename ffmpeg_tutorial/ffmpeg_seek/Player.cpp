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
*/#include "include/Player.h"


int Player::init() {
    spFormatContext = shared_ptr<AVFormatContext>(avformat_alloc_context(), [](AVFormatContext *ctx) {
        avformat_free_context(ctx);
    });

    return RET_OK;
}

int Player::open(string path, int width, int height) {
    AV_RETURN_IF_NULL(spFormatContext, RET_FAIL);
    auto ctx = spFormatContext.get();
    int ret = avformat_open_input(&ctx, path.c_str(), nullptr, nullptr);
    AV_RETURN_IF_NEGATIVE(ret, RET_FAIL, "avformat_open_input");
    return RET_OK;
}

int Player::initVideoFrame(shared_ptr<AVFrame> & videoFrame,shared_ptr<AVCodecContext> &videoCodecCtx) {

    AV_RETURN_IF_NULL(spVideoCodecCtx, RET_FAIL);

    videoFrame = shared_ptr<AVFrame>(av_frame_alloc(), [](AVFrame *) {});

    int ret = av_image_get_buffer_size(videoCodecCtx->pix_fmt, videoCodecCtx->width, videoCodecCtx->height, 1);
    AV_RETURN_IF_NEGATIVE(ret, RET_FAIL, "av_image_get_buffer_size");

    spBuffer = shared_ptr<uint8_t>((uint8_t *) av_malloc(ret), [](uint8_t *p) {
        delete[] p;
    });

    AV_RETURN_IF_NULL(spBuffer, RET_FAIL);
    ret = av_image_fill_arrays(videoFrame->data, videoFrame->linesize, spBuffer.get(), videoCodecCtx->pix_fmt,
                               videoCodecCtx->width, videoCodecCtx->height, 1);

    AV_RETURN_IF_NEGATIVE(ret, RET_FAIL, "av_image_fill_arrays");

    return RET_OK;
}

int
Player::openCodecContext(int *stream_idx, shared_ptr<AVCodecContext> &codec_ctx, shared_ptr<AVFormatContext> &fmt_ctx,
                         enum AVMediaType type) {

    auto ctx = fmt_ctx.get();
    int ret = av_find_best_stream(ctx, type, -1, -1, nullptr, 0);
    AV_RETURN_IF_NEGATIVE(ret, RET_FAIL, "av_find_best_stream");
    auto streamIndex = ret;

    shared_ptr<AVStream> stream = shared_ptr<AVStream>(fmt_ctx->streams[streamIndex]);
    AV_RETURN_IF_NULL(stream, RET_FAIL);

    shared_ptr<AVCodec> codec = shared_ptr<AVCodec>(avcodec_find_decoder(stream->codecpar->codec_id));
    AV_RETURN_IF_NULL(codec, RET_FAIL);

    codec_ctx = shared_ptr<AVCodecContext>(avcodec_alloc_context3(codec.get()));
    AV_RETURN_IF_NULL(codec_ctx, RET_FAIL);

    ret = avcodec_parameters_to_context(codec_ctx.get(), stream->codecpar);
    AV_RETURN_IF_NEGATIVE(ret, RET_FAIL, "avcodec_parameters_to_context");

    ret = avcodec_open2(codec_ctx.get(), codec.get(), nullptr);
    AV_RETURN_IF_NEGATIVE(ret, RET_FAIL, "avcodec_open2");

    *stream_idx = streamIndex;

    return 0;
}

int Player::initVideoSwsContext(shared_ptr<SwsContext> &videoSwsContext, shared_ptr<AVCodecContext> &videoCodecCtx) {

    videoSwsContext = shared_ptr<SwsContext>(
            sws_getContext(
                    videoCodecCtx->width, videoCodecCtx->height, videoCodecCtx->pix_fmt,
                    videoCodecCtx->width, videoCodecCtx->height, AV_PIX_FMT_YUV420P,
                    SWS_BICUBIC, nullptr, nullptr, nullptr),
            sws_freeContext);
    return 0;
}

int Player::initAudioSwrContext(shared_ptr<SwrContext> &audioSwrContext) {
    audioSwrContext = shared_ptr<SwrContext>();
    return 0;
}
