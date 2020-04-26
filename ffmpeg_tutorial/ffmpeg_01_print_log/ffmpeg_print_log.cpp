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

using namespace std;

extern "C"{
#include <libavformat/avformat.h>
}

#include <ffmpeg_log.h>

#ifdef PROJECT_DIR_PATH
string path = string(PROJECT_DIR_PATH) + "/resource/video/video-avi-320x320.avi";
#endif

int main(){

    // 设置日志打印级别
    av_log_set_level(AV_LOG_TRACE);

    // 打印日志

    // 自定义日志打印回调
    av_log_set_callback(log_callback);

    av_log(nullptr,AV_LOG_INFO,"print ffmpeg log\n\n");


    // 设置成默认的打印
//    av_log_set_callback(av_log_default_callback);

    av_log(nullptr,AV_LOG_INFO,"this is  INFO log color");
    av_log(nullptr,AV_LOG_VERBOSE,"this is  VERBOSE log color");
    av_log(nullptr,AV_LOG_DEBUG,"this is DEBUG log color");
    av_log(nullptr,AV_LOG_WARNING,"this is WARNING log color");
    av_log(nullptr,AV_LOG_ERROR,"this is ERROR log color");

    printf("\n-----------华丽的分割线-----------\n\n");


    log("this is macro DEFAULT log color");
    logD("this is macro DEBUG log color");
    logI("this is macro INFO log color");
    logV("this is macro VERBOSE log color");
    logW("this is macro WARNING log color");
    logE("this is macro ERROR log color");

    return 0;
}