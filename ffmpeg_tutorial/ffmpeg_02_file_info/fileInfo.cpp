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

#include "fileInfo.h"

FileInfo::FileInfo(string path) : mPath(path) {
//    av_log_set_level(AV_LOG_INFO);
//    av_log_set_callback(log_callback);
}

FileInfo::~FileInfo() {
    avformat_close_input(&mFormatContext);
    avformat_free_context(mFormatContext);
}

int FileInfo::openInputFile(AVFormatContext *&fmt_ctx, const char *filename) {

    fmt_ctx = avformat_alloc_context();

    int ret = RET_OK;
    if ((ret = avformat_open_input(&fmt_ctx, filename, nullptr, nullptr)) < 0) {
        logE( "Cannot open input file");
        return ret;
    }

    if ((ret = avformat_find_stream_info(fmt_ctx, nullptr)) < 0) {
        logE("Cannot find stream information");
        return ret;
    }

    logI("open input file success");

    // 打印流信息
    av_dump_format(mFormatContext,0,filename,0);

    return ret;
}

void FileInfo::dumpInfo() {
    logI("file path is %s", mFormatContext->filename);
    logI("iformat name is %s", mFormatContext->iformat->name);
    logI("nb_streams is %d", mFormatContext->nb_streams);
    logI("bitrate is %lld", mFormatContext->bit_rate);
    logI("duration is %lld", mFormatContext->duration);
    logI("start time is %lld",mFormatContext->start_time);
}

int FileInfo::openInputFile() {
    return openInputFile(mFormatContext, mPath.c_str());
}
