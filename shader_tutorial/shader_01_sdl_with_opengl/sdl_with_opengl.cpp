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
//#include <GL/glew.h>
#include <SDL.h>

#include <common.h>
#include <log.h>

#include <gl_base.h>

using namespace std;
#define WINDOW_DEFAULT_WIDTH 640
#define WINDOW_DEFAULT_HEIGHT 640


SDL_Window *mWindow;   // Our window handle
SDL_GLContext mContext; // Our opengl context handle


int main() {
    SDL_DisplayMode mMode;

    mMode.w = WINDOW_DEFAULT_WIDTH;
    mMode.h = WINDOW_DEFAULT_HEIGHT;

    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Initilizated SDL Failed: %s", SDL_GetError());
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    mWindow = SDL_CreateWindow("ShaderToy",
                               SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               mMode.w, mMode.h,
                               SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);


    if (!mWindow)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create window: %s",
                     SDL_GetError());
        return -1;
    }

    mContext = SDL_GL_CreateContext(mWindow);

    if (!mContext)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create OpenGL Context: %s",
                     SDL_GetError());
        SDL_DestroyWindow(mWindow);
        return -1;
    }

    int x, y, w, h;
    SDL_GetWindowPosition(mWindow, &x, &y);
    SDL_GL_GetDrawableSize(mWindow, &w, &h);

//    mRect = new Rectangle(y, y + mMode.h, x, x + mMode.w);
//    mDrawRect = new Rectangle(0, h, 0, w);

    if (glewInit() != GLEW_OK)
        exit(EXIT_FAILURE);

//    int ret = SDL_GL_SetSwapInterval(1);
//    cout << ret << endl;

    SDL_Event windowEvent;
    float  red = 0.0f;
    bool bQuit = false;
    while (!bQuit) {
        glClearColor(red, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        while (SDL_PollEvent(&windowEvent)) {
            switch (windowEvent.type) {
                case SDL_QUIT:
                    bQuit = true;
                    break;
                default:
                    break;
            }
        }

//        glClearColor(red, 0.0f, 0.0f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        red+= 0.01f;
        SDL_GL_SwapWindow(mWindow);

    }

    return 0;
}
