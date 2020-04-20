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
#include <libavutil/log.h>
}

#include <log.h>

void log_callback(void* ptr, int level, const char* fmt, va_list vaList){
    logging(fmt,vaList);
}

int main(){

    cout << "version : " << avcodec_version() << endl;

    // 设置日志打印级别
    av_log_set_level(AV_LOG_INFO);

    // 打印日志
    av_log(nullptr,AV_LOG_INFO,"print ffmpeg log\n");

    av_log_set_callback(log_callback);

    av_log(nullptr,AV_LOG_INFO,"print log by callback\n");


    return 0;
}