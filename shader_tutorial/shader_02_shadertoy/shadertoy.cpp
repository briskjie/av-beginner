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

#include <display.h>

using namespace std;
#define WINDOW_DEFAULT_WIDTH 640
#define WINDOW_DEFAULT_HEIGHT 640

#ifdef PROJECT_DIR_PATH
string vertex_path = string(PROJECT_DIR_PATH) + "/base/shader_res/vertex.glsl";
string fragment_path = string(PROJECT_DIR_PATH) + "/base/shader_res/fragment.glsl";
string texture_path = string(PROJECT_DIR_PATH) + "/resource/image/image-jpg-640x426.jpg";
#endif

#include <mesh.h>
#include <shader.h>
#include <texture.h>

int main() {

    Display display(WINDOW_DEFAULT_WIDTH,WINDOW_DEFAULT_HEIGHT,"opengl");

    vector<Vertex> vertices = {
            Vertex(glm::vec3(-1.0f,-1.0f,0.0f),glm::vec2(0.0f,0.0f)),
            Vertex(glm::vec3(-1.0f,1.0f,0.0f),glm::vec2(0.0f,1.0f)),
            Vertex(glm::vec3(1.0f,1.0f,0.0f),glm::vec2(1.0f,1.0f)),
            Vertex(glm::vec3(1.0f,-1.0f,0.0f),glm::vec2(1.0f,0.0f))
    };

    vector<GLuint> indices = {
            0,1,2,
            0,2,3
    };

    Mesh mesh(vertices,indices);

    Shader shader(vertex_path,fragment_path);
    Texture texture(texture_path);

    SDL_Event windowEvent;
    bool bQuit = false;
    while (!bQuit) {
        while (SDL_PollEvent(&windowEvent)) {
            switch (windowEvent.type) {
                case SDL_QUIT:
                    bQuit = true;
                    break;
                default:
                    break;
            }
        }
        display.clear(1.0, 0.0f, 0.0f, 1.0f);
        shader.bind();
        texture.bind();
        mesh.Draw();
        display.swapBuffers();
    }

    return 0;
}
