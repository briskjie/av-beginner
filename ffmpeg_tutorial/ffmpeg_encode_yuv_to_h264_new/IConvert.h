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

#ifndef AV_BEGINNER_ICONVERT_H
#define AV_BEGINNER_ICONVERT_H

#include <iostream>
#include <ffmpeg_log.h>

using namespace std;

class IConvert {

public:

    IConvert() = default;

    // 基类的析构函数使用 virtual,可以避免内存泄漏，因为会通过基类的指针指向子类对象，实现多态。
    // 如果没有 virtual，那么只会调用基类的析构，不会调用子类的析构
    // 如果使用智能指针 shared_ptr 的话，子类和基类的析构都会被调用到
    virtual ~IConvert() {
        log("call base class virtual method");
    }

    virtual int open(string input = "", string output = "", int width = -1, int height = -1) ;

    virtual int initContext() ;

    virtual int initBuffer();

    virtual int writeHeader();

    virtual int encodeData() ;

    virtual int writeTrailer() ;

};

#endif //AV_BEGINNER_ICONVERT_H
