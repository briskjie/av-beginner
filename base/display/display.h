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

#ifndef AV_BEGINNER_DISPLAY_H
#define AV_BEGINNER_DISPLAY_H

#include <SDL.h>
//#include <GL/glew.h>
#include <log.h>
#include <iostream>
#include <string>
//#include <glad_gl_3_3/glad_gl_3_3.h>
//#include <glad/glad.h>
#include <gl_base.h>

using namespace std;

class Display {

public:
    Display(int width,int height,const string& title);
    ~Display();

    void clear(float r, float g, float b, float a);
    void swapBuffers();

private:
    SDL_Window *mWindow;
    SDL_GLContext mGLContext;

//    // 拷贝构造函数
//    Display(const Display &display){
//
//    }
//
//    // 移动构造函数
//    Display(Display && display){
//
//    }
//
//    // 拷贝赋值运算符
//    Display & operator=(const Display& display){
//
//    }
//
//    // 移动赋值运算符
//    Display &operator=(const Display&& display){
//
//    }
};


#endif //AV_BEGINNER_DISPLAY_H
