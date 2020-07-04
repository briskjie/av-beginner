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
#include <iostream>
#include "decoder.h"

#include "display_.h"
#include "SDLWrapper.h"

using namespace std;
#ifdef PROJECT_DIR_PATH
string path = string(PROJECT_DIR_PATH) + "/resource/video/video-mp4-640x360.mp4";
#endif

static bool send(AVCodecContext *codec_context, AVPacket *packet) {
    auto ret = avcodec_send_packet(codec_context, packet);
    return !(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF);
}

static bool receive(AVCodecContext *codec_context, AVFrame *frame) {
    auto ret = avcodec_receive_frame(codec_context, frame);

    log("receive ret is %d",ret);

    return !(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF);
}

//
//int sdl_thread_handle_refreshing(void * opaque) {
//    SDL_Event sdlEvent;
//    int frame_rate = *(int*)opaque;
//
//    int interval = (frame_rate > 0) ? 1000 / frame_rate : 40;
//    Log::print("frame rate %d FPS, refresh interval %d ms\n", frame_rate, interval);
//
//    while (!isExit) {
//        if (!isPause){
//            sdlEvent.type = SDL_USEREVENT_REFRESH;
//            SDL_PushEvent(&sdlEvent);
//        }
//        SDL_Delay(interval);
//    }
//    return 0;
//}

static bool isExit = false;

static bool isPause = false;

#define SDL_USEREVENT_REFRESH  (SDL_USEREVENT + 1)



int sdl_thread_handle_refreshing(void * opaque) {
    SDL_Event sdlEvent;
    int frame_rate = *(int*)opaque;

//    int interval = (frame_rate > 0) ? 1000 / frame_rate : 40;
//    log("frame rate %d FPS, refresh interval %d ms\n", frame_rate, interval);

    while (!isExit) {
        if (!isPause){
            sdlEvent.type = SDL_USEREVENT_REFRESH;
            SDL_PushEvent(&sdlEvent);
            log("push event");
        }
//        SDL_Delay(interval);
        SDL_Delay(40);
    }
    return 0;
}


int main() {

    shared_ptr<Decoder> decoder = make_shared<Decoder>(path);
    decoder->openInputFile();
    decoder->openCodec();
    decoder->dumpInfo();

    decoder->startDecodeThread();

//    return 0;


    int count = 0;

    while(false) {

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
//                log("push packet and queue size is %d", decoder->packetQueue->count());
                break;
            }
        }

//        log("video count is %d", count);
    }


    std::function<void(void *, int)> save = pgm_save;




//    for (;;) {
//        std::unique_ptr<AVFrame, std::function<void(AVFrame *)>> frame_decoded{
//                av_frame_alloc(), [](AVFrame *f) {
////                    av_frame_unref(f);
////                    av_frame_free(&f);
////                    log("delete frame in read thread");
//                }
//        };
//
//        std::unique_ptr<AVPacket, std::function<void(AVPacket *)>> packet{
//                nullptr, [](AVPacket *p) {
//                    av_packet_unref(p);
//                    av_packet_free(&p);
//                    log("delete packet in read thread");
//                }
//        };
//
//        log("pop packet");
//        if (!decoder->packetQueue->pop(packet)) {
//            break;
//        }
//
//        bool sent = false;
//        while (!sent) {
//
//            sent = send(decoder->mVideoDecContext, packet.get());
//
//            while (receive(decoder->mVideoDecContext, frame_decoded.get())) {
//
//                save(frame_decoded.get(), decoder->mVideoDecContext->frame_number);
//
//
//            }
//        }
//    }

//    unique_ptr<Display> display = make_unique<Display>(decoder->mWidth, decoder->mHeight);
//    std::function<void(void *, int)> save = pgm_save;
//
    SDLWrapper *sdlWrapper = new SDLWrapper;
    SDL_Thread *sdlThread;

    int width = decoder->mVideoDecContext->width;
    int height = decoder->mVideoDecContext->height;

    SDL_Event sdlEvent;

    sdlWrapper->init();
    sdlWrapper->createWindow("window", width , height);
    sdlWrapper->createRenderer();
    sdlWrapper->createTexture(width, height);
    SDL_SetRenderDrawColor(sdlWrapper->sdl_renderer, 255, 0, 0, 255);

    int framerate = 40;
    sdlThread = SDL_CreateThread(sdl_thread_handle_refreshing, "name", &framerate);

    while (true){
        SDL_WaitEvent(&sdlEvent);
        if (sdlEvent.type == SDL_USEREVENT_REFRESH) {

            log("enter SDL_USEREVENT_REFRESH");

            std::unique_ptr<AVFrame, std::function<void(AVFrame *)>> frame_decoded{
                    av_frame_alloc(), [](AVFrame *f) {
//                        av_frame_unref(f);
//                        av_frame_free(&f);
                        log("delete frame in read thread");
                    }
            };

//            AVFrame *videoFrame;
//
//            videoFrame = av_frame_alloc();

            std::unique_ptr<AVPacket, std::function<void(AVPacket *)>> packet{
                    nullptr, [](AVPacket *p) {
                        av_packet_unref(p);
                        av_packet_free(&p);
                        log("delete packet in read thread");
                    }
            };

            log("pop packet");
            if (!decoder->packetQueue->pop(packet)) {

                log("end queue and back");

                break;
            }


//            auto ret = avcodec_send_packet(decoder->mVideoDecContext, packet.get());
//
//            if (ret < 0){
//                logE("error send packet for decoding and ret is %d\n",ret);
////                return RET_FAIL;
//            }
//            while (ret >= 0){
//                ret = avcodec_receive_frame(decoder->mVideoDecContext, frame_decoded.get());
//                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF){
//                    log("ret is %d and again",ret);
////                    return RET_OK;
//                } else if (ret < 0){
//                    logE("error during decoding and ret is %d\n",ret);
////                    return RET_FAIL;
//                } else {
//                    log("save frame number and num is %d",decoder->mVideoDecContext->frame_number);
////                    save(frame_decoded.get(), decoder->mVideoDecContext->frame_number);
//
//
//                    SDL_UpdateYUVTexture(sdlWrapper->sdl_texture, nullptr,
//                                         frame_decoded->data[0],frame_decoded->linesize[0],
//                                         frame_decoded->data[1],frame_decoded->linesize[1],
//                                         frame_decoded->data[2],frame_decoded->linesize[2]);
//
//
//
//                    SDL_RenderClear(sdlWrapper->sdl_renderer);
//
//                    SDL_RenderCopy(sdlWrapper->sdl_renderer,sdlWrapper->sdl_texture, nullptr, nullptr);
//
//                    SDL_RenderPresent(sdlWrapper->sdl_renderer);
//
//                }
//            }


            bool sent = false;
            while (!sent) {

                sent = send(decoder->mVideoDecContext, packet.get());

                while (receive(decoder->mVideoDecContext, frame_decoded.get())) {
//                    save(frame_decoded.get(), decoder->mVideoDecContext->frame_number);

                    SDL_UpdateYUVTexture(sdlWrapper->sdl_texture, nullptr,
                                         frame_decoded.get()->data[0],frame_decoded.get()->linesize[0],
                                         frame_decoded.get()->data[1],frame_decoded.get()->linesize[1],
                                         frame_decoded.get()->data[2],frame_decoded.get()->linesize[2]);


                    SDL_RenderClear(sdlWrapper->sdl_renderer);

                    SDL_RenderCopy(sdlWrapper->sdl_renderer, sdlWrapper->sdl_texture, nullptr, nullptr);

                    SDL_RenderPresent(sdlWrapper->sdl_renderer);

                }
            }
        }else if (sdlEvent.type == SDL_KEYDOWN){
            if (sdlEvent.key.keysym.sym == SDLK_SPACE){
                isPause = !isPause;
            }
        } else if (sdlEvent.type == SDL_QUIT){
            isExit = !isExit;
            break;
        }
    }

//    for (;;) {
//        std::unique_ptr<AVFrame, std::function<void(AVFrame *)>> frame_decoded{
//                av_frame_alloc(), [](AVFrame *f) {
//                    av_frame_unref(f);
//                    av_frame_free(&f);
//                    log("delete frame in read thread");
//                }
//        };
//
//        std::unique_ptr<AVPacket, std::function<void(AVPacket *)>> packet{
//                nullptr, [](AVPacket *p) {
//                    av_packet_unref(p);
//                    av_packet_free(&p);
//                    log("delete packet in read thread");
//                }
//        };
//
//        log("pop packet");
//        if (!decoder->packetQueue->pop(packet)) {
//            break;
//        }
//
//        bool sent = false;
//        while (!sent) {
//
//            sent = send(decoder->mVideoDecContext, packet.get());
//
//            while (receive(decoder->mVideoDecContext, frame_decoded.get())) {
//                save(frame_decoded.get(), decoder->mVideoDecContext->frame_number);
//
//
//
//            }
//        }
//    }



    for (int i = 0; i > -1; ++i) {

        display->input();

        if (display->get_quit()) {
            log("quit display");
            break;
        } else if (display->get_play()) {

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
                log("pop end ");
                break;
            }

            bool sent = false;
            while (!sent) {

                sent = send(decoder->mVideoDecContext, packet.get());

                while (receive(decoder->mVideoDecContext, frame_decoded.get())) {
//                    save(frame_decoded.get(), decoder->mVideoDecContext->frame_number);
                    log("refresh display and %d %d %d",
                        frame_decoded->linesize[0],
                        frame_decoded->linesize[1],
                        frame_decoded->linesize[2]);

//                    decoder->display->refresh({
//                                                      frame_yuv->data[0],
//                                                      frame_yuv->data[1],
//                                                      frame_yuv->data[2]},
//                                              {
//                                                      static_cast<size_t>(frame_yuv->linesize[0]),
//                                                      static_cast<size_t>(frame_yuv->linesize[1]),
//                                                      static_cast<size_t>(frame_yuv->linesize[2])}
//                    );
//
                    decoder->display->refresh({
                                                      frame_decoded->data[0],
                                                      frame_decoded->data[1],
                                                      frame_decoded->data[2]},
                                              {
                                                      static_cast<size_t>(frame_decoded->linesize[0]),
                                                      static_cast<size_t>(frame_decoded->linesize[1]),
                                                      static_cast<size_t>(frame_decoded->linesize[2])}
                    );
                    log("refresh");

//                    SDL_UpdateYUVTexture(decoder->display->texture_.get(), nullptr,
//                                         frame_decoded->data[0], frame_decoded->linesize[0],
//                                         frame_decoded->data[1], frame_decoded->linesize[1],
//                                         frame_decoded->data[2], frame_decoded->linesize[2]);
//
//                    SDL_RenderClear(decoder->display->renderer_.get());
//                    SDL_RenderCopy(decoder->display->renderer_.get(), decoder->display->texture_.get(), nullptr, nullptr);
//                    SDL_RenderPresent(decoder->display->renderer_.get());
//                    SDL_Delay(40);
                }
            }
        }
    }


    return 0;
}