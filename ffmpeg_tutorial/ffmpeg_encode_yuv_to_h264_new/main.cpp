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
#include <unordered_map>
#include <map>

using namespace std;

#include "YuvConvert.h"
#include "IConvert.h"
#include <ffmpeg_log.h>


#ifdef PROJECT_DIR_PATH
string path = string(PROJECT_DIR_PATH) + "/resource/gen/file-yuv-640x360.yuv";
#endif


int main() {

    shared_ptr<IConvert> convert = make_shared<YUVConvert>();

    string output_path = string(PROJECT_DIR_PATH) + "/resource/gen/file-yuv-640x360.h264";

    log("input path is %s", path.c_str());
    log("output path is %s", output_path.c_str());

    AV_CALL(convert->open(path, output_path, 640, 360));

    AV_CALL(convert->initContext());

    AV_CALL(convert->initBuffer());
    AV_CALL(convert->writeHeader());
    AV_CALL(convert->encodeData());
    AV_CALL(convert->writeTrailer());


    return RET_OK;
}