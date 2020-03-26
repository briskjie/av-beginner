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

#include <shader.h>


Shader::Shader(const string &vertexFile, const string & fragmentFile) {
    mProgram = glCreateProgram();
    mShaders[0] = createShader(loadShader(vertexFile), GL_VERTEX_SHADER);
    mShaders[1] = createShader(loadShader(fragmentFile), GL_FRAGMENT_SHADER);

    for (int i = 0; i < 2; ++i) {
        glAttachShader(mProgram,mShaders[i]);
    }

    glBindAttribLocation(mProgram,0,"a_Position");

    glLinkProgram(mProgram);
    checkShaderError(mProgram,GL_LINK_STATUS, true,"Error linking shader program");

    glValidateProgram(mProgram);
    checkShaderError(mProgram,GL_LINK_STATUS, true,"Invalid shader program");

//    mUniforms[0] = glGetUniformLocation(mProgram,"MVP");
//    mColor = glGetUniformLocation(mProgram,"u_Color");
//    glUniform4f(mColor,0.0f,1.0f,1.0f,1.0f);

}

void Shader::bind() {
//    glUniform4f(mUniforms[1],0.0f,1.0f,1.0f,1.0f);
    glUseProgram(mProgram);
}

string Shader::loadShader(const string & filePath) {
    ifstream file;
    file.open(filePath);

    string output;
    string line;
    if (file.is_open()) {
        while (file.good()) {
            getline(file, line);
            output.append(line + "\n");
        }
    } else {
        AVLOG("load shader failed");
    }

    AVLOG("Load shaderr is %s",output.c_str());

    return output;
}

GLuint Shader::createShader(const string &text, unsigned int type) {
    GLuint shader = glCreateShader(type);
    if (shader == 0) {
        AVLOG("create shader failed and type is %d", type);
    }

    const GLchar *p[1];
    p[0] = text.c_str();
    GLint length[1];
    length[0] = text.length();

    GLint success;
    glShaderSource(shader, 1, p, length);
    glCompileShader(shader);

//    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    checkShaderError(shader, GL_COMPILE_STATUS, false, "Error compile shader");
    return 0;
}

void Shader::checkShaderError(GLuint shader, GLuint flag, bool isProgram, const string &errorMsg) {
    GLint success = 0;
    GLchar error[1024] = {0};
    if (isProgram) {
        glGetProgramiv(shader, flag, &success);
    } else {
        glGetShaderiv(shader, flag, &success);
    }

    if (success == GL_FALSE) {
        if (isProgram) {
            glGetProgramInfoLog(shader, sizeof(error), nullptr, error);
        } else {
            glGetShaderInfoLog(shader, sizeof(error), nullptr, error);
        }
        AVLOG("%s : %s", errorMsg.c_str(), error);
    } else {
        AVLOG("%s success",errorMsg.c_str());
    }

}

Shader::~Shader() {

}
