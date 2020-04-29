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
#include "decoder.h"
#include <iostream>

#include <ffmpeg_log.h>

using namespace std;
#ifdef PROJECT_DIR_PATH
string path = string(PROJECT_DIR_PATH) + "/resource/video/video-mp4-640x360.mp4";
#endif


int main(){

    Decoder decoder = Decoder(path);
    decoder.openInputFile();
    decoder.openCodec();
    decoder.dumpInfo();

    decoder.decodeVideo();

    return 0;
}