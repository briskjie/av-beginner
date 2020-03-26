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

#ifndef AV_BEGINNER_SHADER_H
#define AV_BEGINNER_SHADER_H

#include <gl_base.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <log.h>

//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>

using namespace std;

class Shader {

public:
    Shader(const string& vertexFile, const string & fragmentFile);
    ~Shader();

    void bind();

private:
    GLuint mProgram;
    GLuint mShaders[2];
    GLuint mUniforms[3];

    GLuint mColor;
    string loadShader(const string & filePath);
    GLuint createShader(const string& text , unsigned int type);
    void checkShaderError(GLuint shader,GLuint flag,bool isProgram,const string &errorMsg);
};


#endif //AV_BEGINNER_SHADER_H
