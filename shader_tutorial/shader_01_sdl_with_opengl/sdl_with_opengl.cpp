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
#include <GL/glew.h>
#include <SDL.h>

struct { // screen size structure
    int w;
    int h;
} screen;

SDL_Window * mainwindow;   // Our window handle
SDL_GLContext maincontext; // Our opengl context handle


void Render(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    {
        glColor3f(1.0, 0.0, 0.0);
        glVertex2f(0, .5);
        glColor3f(0.0, 1.0, 0.0);
        glVertex2f(-.5, -.5);
        glColor3f(0.0, 0.0, 1.0);
        glVertex2f(.5, -.5);
    }
    glEnd();
}

int main(){

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) { /* Initialize SDL's Video subsystem */
        return -1;
    }


    SDL_DisplayMode mode;
    SDL_GetDesktopDisplayMode(0, &mode);

    screen.w = (int)(mode.h - 200) / 1.5;
    screen.h = mode.h - 200;

    mainwindow = SDL_CreateWindow("Simple texture moving", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  screen.w, screen.h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    maincontext = SDL_GL_CreateContext(mainwindow);
    glCreateShader(GL_FRAGMENT_SHADER);
//    glViewport(0, 0, screen.w, screen.h);
//    glClear(GL_COLOR_BUFFER_BIT);

    Render();
    SDL_GL_SwapWindow(mainwindow);
    return 0;
}
