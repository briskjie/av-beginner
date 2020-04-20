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

#ifndef AV_BEGINNER_MESH_H
#define AV_BEGINNER_MESH_H

#include <gl_base.h>
#include <iostream>
#include <vector>
#include <common.h>

using namespace std;

struct Vertex{
public:

    Vertex(const glm::vec3 & pos){
        this->Position = pos;
    }

    Vertex(const glm::vec3 & pos,const glm::vec2 & texture){
        this->Position = pos;
        this->TexCoords = texture;
    }
//private:
    glm::vec3 Position;
//    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh {

public:

    vector<Vertex> vertices;
    vector<GLuint> indices;

    Mesh(vector<Vertex> & vertices, vector<GLuint> & indices);

    ~Mesh();

    void Draw();

private:
    GLuint VAO,VBO,EBO;

    void setupMesh();

};


#endif //AV_BEGINNER_MESH_H
