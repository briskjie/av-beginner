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

extern "C"{
#include <libavutil/log.h>
}

#ifndef AV_BEGINNER_FFMPEG_LOG_H
#define AV_BEGINNER_FFMPEG_LOG_H

#define RET_OK 0
#define RET_FAIL 1

#define LOG_NONE    "\e[0m"
#define LOG_BLACK   "\e[0;30m"
#define LOG_RED     "\e[0;31m"
#define LOG_GREEN   "\e[0;32m"
#define LOG_YELLOW  "\e[0;33m"
#define LOG_BLUE    "\e[0;34m"
#define LOG_MAGENTA "\e[0;35m"
#define LOG_CYAN    "\e[0;36m"
#define LOG_WHITE   "\e[1;37m"

#define logD(format,...) logging(LOG_GREEN,format,##__VA_ARGS__)

#define logI(format,...) logging(LOG_WHITE,format,##__VA_ARGS__)

#define logV(format,...) logging(LOG_BLUE,format,##__VA_ARGS__)

#define logW(format,...) logging(LOG_YELLOW,format,##__VA_ARGS__)

#define logE(format,...) logging(LOG_RED,format,##__VA_ARGS__)

#define log(format,...)  logging(LOG_MAGENTA,format,##__VA_ARGS__)

#define AV_CALL(func)                                                           \
    if (RET_OK != (func)){                                                      \
        logE("call error file[%s],line[%d]",__FILE__,__LINE__);                  \
    }                                                                           \

static void logging(const char * color ,const char *fmt, va_list vaList)
{
    printf("%s [av-beginner]: ",color);
    vprintf( fmt, vaList );
    printf(LOG_NONE "\n" );
}

static void logging(const char * color ,const char *fmt, ...)
{
    va_list args;
    va_start( args, fmt );
    logging(color,fmt,args);
    va_end(args);
}

// 在头文件中定义的函数，会被重复定义
// 需要加 static 或者 inline 来解决
static void log_callback(void *ptr, int level, const char *fmt, va_list vaList) {
    switch (level) {
        case AV_LOG_DEBUG:
            logD(fmt, vaList);
            break;
        case AV_LOG_VERBOSE:
            logV(LOG_MAGENTA, fmt, vaList);

            break;
        case AV_LOG_INFO:
            logI(fmt, vaList);
            break;
        case AV_LOG_WARNING:
            logW(fmt, vaList);
            break;
        case AV_LOG_ERROR:
            logE(fmt, vaList);
            break;
        default:
            log(fmt, vaList);
            break;
    }
}

#endif //AV_BEGINNER_FFMPEG_LOG_H
