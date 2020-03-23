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

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

struct Vertex{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec3 TexCoords;
};


struct Texture{
    GLuint id;
    string type;
};

class Mesh {
public:
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;

    Mesh(vector<Vertex> vertices,vector<GLuint> indices,vector<Texture> texture);
//    void Draw(shader shader);

private:
    GLuint VAO,VBO,EBO;
    void setupMesh();
};


#endif //AV_BEGINNER_MESH_H
