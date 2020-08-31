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
#include "include/Player.h"
#include "include/SeekPlayer.h"
#include "ffmpeg_log.h"
#ifdef PROJECT_DIR_PATH
string path = string(PROJECT_DIR_PATH) + "/resource/video/video-mp4-768x432.mp4";
#endif

int test(){
    return -1;
}

class SP{

};
int main(){

    unique_ptr<Player> player = make_unique<SeekPlayer>();

    AV_CALL(player->init());
    AV_CALL(player->open(path.c_str()));
//    AV_CALL(player->)

//    shared_ptr<uint8_t> test  = shared_ptr<uint8_t>(new uint8_t[10],[](uint8_t* p){
//        delete[] p ;
//    });


    return 0;
}
