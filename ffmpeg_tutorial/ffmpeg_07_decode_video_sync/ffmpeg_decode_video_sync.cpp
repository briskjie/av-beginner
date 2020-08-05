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


static bool isExit = false;

static bool isPause = false;

#define SDL_USEREVENT_REFRESH  (SDL_USEREVENT + 1)



int sdl_thread_handle_refreshing(void * opaque) {
    SDL_Event sdlEvent;

    while (!isExit) {
        if (!isPause){
            sdlEvent.type = SDL_USEREVENT_REFRESH;
            SDL_PushEvent(&sdlEvent);
//            log("push event");
        }
        SDL_Delay(40);
    }
    return 0;
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
            decoder->frameQueue->finished();
            break;
        }

        bool sent = false;
        while (!sent) {

            sent = send(decoder->mVideoDecContext, packet.get());

            while (receive(decoder->mVideoDecContext, frame_decoded.get())) {
                log("push frame");
                decoder->frameQueue->push(move(frame_decoded));
                log("push frame and size is %d",decoder->frameQueue->count());
                break;
//                save(frame_decoded.get(), decoder->mVideoDecContext->frame_number);
            }
        }
    }
    log("finish read thread");
}

int main() {



    shared_ptr<Decoder> decoder = make_shared<Decoder>(path);
    decoder->openInputFile();
    decoder->openCodec();
    decoder->dumpInfo();

//    decoder->startDemultiplex();
//    decoder->startDecodeVideo();

    std::thread demux(demux_thread,decoder.get());
    std::thread decode(decode_video_thread,decoder.get());

    SDLWrapper *sdlWrapper = new SDLWrapper;

    int width = decoder->mVideoDecContext->width;
    int height = decoder->mVideoDecContext->height;

    SDL_Event sdlEvent;

    sdlWrapper->init();
    sdlWrapper->createWindow("window", width , height);
    sdlWrapper->createRenderer();
    sdlWrapper->createTexture(width, height);

    SDL_SetRenderDrawColor(sdlWrapper->sdl_renderer, 255, 0, 0, 255);

    int framerate = 40;
   SDL_CreateThread(sdl_thread_handle_refreshing, "name", &framerate);

    while (true){
        SDL_WaitEvent(&sdlEvent);
        if (sdlEvent.type == SDL_USEREVENT_REFRESH) {

            cout << "receive SDL_USEREVENT_REFRESH" << endl;

            std::unique_ptr<AVFrame, std::function<void(AVFrame *)>> frame_decoded{
                    nullptr, [](AVFrame *f) {
                        av_frame_unref(f);
                        av_frame_free(&f);
                        log("delete frame in read thread");
                    }
            };

            if (decoder->frameQueue->pop(frame_decoded)){
                                    sdlWrapper->refresh({
                                                frame_decoded->data[0],
                                                frame_decoded->data[1],
                                                frame_decoded->data[2]},
                                        {
                                                static_cast<size_t>(frame_decoded->linesize[0]),
                                                static_cast<size_t>(frame_decoded->linesize[1]),
                                                static_cast<size_t>(frame_decoded->linesize[2])});
//                SDL_Delay(40);
            } else {
                log("no available frame");
            }


//            bool sent = false;
//            while (!sent) {
//
//                sent = send(decoder->mVideoDecContext, packet.get());
//
//                while (receive(decoder->mVideoDecContext, frame_decoded.get())) {
//
//                    sdlWrapper->refresh({
//                                                frame_decoded->data[0],
//                                                frame_decoded->data[1],
//                                                frame_decoded->data[2]},
//                                        {
//                                                static_cast<size_t>(frame_decoded->linesize[0]),
//                                                static_cast<size_t>(frame_decoded->linesize[1]),
//                                                static_cast<size_t>(frame_decoded->linesize[2])});
//
//
//                    SDL_Delay(40);
//                }
//            }

        }else if (sdlEvent.type == SDL_KEYDOWN){
            if (sdlEvent.key.keysym.sym == SDLK_SPACE){
                isPause = !isPause;
            }
        } else if (sdlEvent.type == SDL_QUIT){
            isExit = !isExit;
            break;
        }
    }
    return 0;
}