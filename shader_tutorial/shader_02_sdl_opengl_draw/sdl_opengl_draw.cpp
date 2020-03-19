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
#include <SDL.h>
#include <SDL_opengl.h>
#include <string>

using namespace std;

#ifdef PROJECT_DIR_PATH
string path = string(PROJECT_DIR_PATH) + "/resource/image/hello.bmp";
#endif

typedef struct GL_Context
{
#define SDL_PROC(ret,func,params) ret (APIENTRY *func) params;
#include "../src/render/opengl/SDL_glfuncs.h"
#undef SDL_PROC
} GL_Context;


/* Undefine this if you want a flat cube instead of a rainbow cube */
#define SHADED_CUBE

static SDLTest_CommonState *state;
static SDL_GLContext context;
static GL_Context ctx;

static int LoadContext(GL_Context * data)
{
#if SDL_VIDEO_DRIVER_UIKIT
#define __SDL_NOGETPROCADDR__
#elif SDL_VIDEO_DRIVER_ANDROID
#define __SDL_NOGETPROCADDR__
#elif SDL_VIDEO_DRIVER_PANDORA
#define __SDL_NOGETPROCADDR__
#endif

#if defined __SDL_NOGETPROCADDR__
#define SDL_PROC(ret,func,params) data->func=func;
#else
#define SDL_PROC(ret,func,params) \
    do { \
        data->func = SDL_GL_GetProcAddress(#func); \
        if ( ! data->func ) { \
            return SDL_SetError("Couldn't load GL function %s: %s", #func, SDL_GetError()); \
        } \
    } while ( 0 );
#endif /* __SDL_NOGETPROCADDR__ */

#include "../src/render/opengl/SDL_glfuncs.h"
#undef SDL_PROC
    return 0;
}


/* Call this instead of exit(), so we can clean up SDL: atexit() is evil. */
static void
quit(int rc)
{
    if (context) {
        /* SDL_GL_MakeCurrent(0, NULL); *//* doesn't do anything */
        SDL_GL_DeleteContext(context);
    }
    SDLTest_CommonQuit(state);
    exit(rc);
}

static void
Render()
{
    static float color[8][3] = {
            {1.0, 1.0, 0.0},
            {1.0, 0.0, 0.0},
            {0.0, 0.0, 0.0},
            {0.0, 1.0, 0.0},
            {0.0, 1.0, 1.0},
            {1.0, 1.0, 1.0},
            {1.0, 0.0, 1.0},
            {0.0, 0.0, 1.0}
    };
    static float cube[8][3] = {
            {0.5, 0.5, -0.5},
            {0.5, -0.5, -0.5},
            {-0.5, -0.5, -0.5},
            {-0.5, 0.5, -0.5},
            {-0.5, 0.5, 0.5},
            {0.5, 0.5, 0.5},
            {0.5, -0.5, 0.5},
            {-0.5, -0.5, 0.5}
    };

    /* Do our drawing, too. */
    ctx.glClearColor(0.0, 0.0, 0.0, 1.0);
    ctx.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ctx.glBegin(GL_QUADS);

#ifdef SHADED_CUBE
    ctx.glColor3fv(color[0]);
    ctx.glVertex3fv(cube[0]);
    ctx.glColor3fv(color[1]);
    ctx.glVertex3fv(cube[1]);
    ctx.glColor3fv(color[2]);
    ctx.glVertex3fv(cube[2]);
    ctx.glColor3fv(color[3]);
    ctx.glVertex3fv(cube[3]);

    ctx.glColor3fv(color[3]);
    ctx.glVertex3fv(cube[3]);
    ctx.glColor3fv(color[4]);
    ctx.glVertex3fv(cube[4]);
    ctx.glColor3fv(color[7]);
    ctx.glVertex3fv(cube[7]);
    ctx.glColor3fv(color[2]);
    ctx.glVertex3fv(cube[2]);

    ctx.glColor3fv(color[0]);
    ctx.glVertex3fv(cube[0]);
    ctx.glColor3fv(color[5]);
    ctx.glVertex3fv(cube[5]);
    ctx.glColor3fv(color[6]);
    ctx.glVertex3fv(cube[6]);
    ctx.glColor3fv(color[1]);
    ctx.glVertex3fv(cube[1]);

    ctx.glColor3fv(color[5]);
    ctx.glVertex3fv(cube[5]);
    ctx.glColor3fv(color[4]);
    ctx.glVertex3fv(cube[4]);
    ctx.glColor3fv(color[7]);
    ctx.glVertex3fv(cube[7]);
    ctx.glColor3fv(color[6]);
    ctx.glVertex3fv(cube[6]);

    ctx.glColor3fv(color[5]);
    ctx.glVertex3fv(cube[5]);
    ctx.glColor3fv(color[0]);
    ctx.glVertex3fv(cube[0]);
    ctx.glColor3fv(color[3]);
    ctx.glVertex3fv(cube[3]);
    ctx.glColor3fv(color[4]);
    ctx.glVertex3fv(cube[4]);

    ctx.glColor3fv(color[6]);
    ctx.glVertex3fv(cube[6]);
    ctx.glColor3fv(color[1]);
    ctx.glVertex3fv(cube[1]);
    ctx.glColor3fv(color[2]);
    ctx.glVertex3fv(cube[2]);
    ctx.glColor3fv(color[7]);
    ctx.glVertex3fv(cube[7]);
#else /* flat cube */
    ctx.glColor3f(1.0, 0.0, 0.0);
    ctx.glVertex3fv(cube[0]);
    ctx.glVertex3fv(cube[1]);
    ctx.glVertex3fv(cube[2]);
    ctx.glVertex3fv(cube[3]);

    ctx.glColor3f(0.0, 1.0, 0.0);
    ctx.glVertex3fv(cube[3]);
    ctx.glVertex3fv(cube[4]);
    ctx.glVertex3fv(cube[7]);
    ctx.glVertex3fv(cube[2]);

    ctx.glColor3f(0.0, 0.0, 1.0);
    ctx.glVertex3fv(cube[0]);
    ctx.glVertex3fv(cube[5]);
    ctx.glVertex3fv(cube[6]);
    ctx.glVertex3fv(cube[1]);

    ctx.glColor3f(0.0, 1.0, 1.0);
    ctx.glVertex3fv(cube[5]);
    ctx.glVertex3fv(cube[4]);
    ctx.glVertex3fv(cube[7]);
    ctx.glVertex3fv(cube[6]);

    ctx.glColor3f(1.0, 1.0, 0.0);
    ctx.glVertex3fv(cube[5]);
    ctx.glVertex3fv(cube[0]);
    ctx.glVertex3fv(cube[3]);
    ctx.glVertex3fv(cube[4]);

    ctx.glColor3f(1.0, 0.0, 1.0);
    ctx.glVertex3fv(cube[6]);
    ctx.glVertex3fv(cube[1]);
    ctx.glVertex3fv(cube[2]);
    ctx.glVertex3fv(cube[7]);
#endif /* SHADED_CUBE */

    ctx.glEnd();

    ctx.glMatrixMode(GL_MODELVIEW);
    ctx.glRotatef(5.0, 1.0, 1.0, 1.0);
}

int
main(int argc, char *argv[])
{
    int fsaa, accel;
    int value;
    int i, done;
    SDL_DisplayMode mode;
    SDL_Event event;
    Uint32 then, now, frames;
    int status;
    int dw, dh;
    int swap_interval = 0;

    /* Enable standard application logging */
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

    /* Initialize parameters */
    fsaa = 0;
    accel = -1;

    /* Initialize test framework */
    state = SDLTest_CommonCreateState(argv, SDL_INIT_VIDEO);
    if (!state) {
        return 1;
    }
    for (i = 1; i < argc;) {
        int consumed;

        consumed = SDLTest_CommonArg(state, i);
        if (consumed == 0) {
            if (SDL_strcasecmp(argv[i], "--fsaa") == 0 && i+1 < argc) {
                fsaa = atoi(argv[i+1]);
                consumed = 2;
            } else if (SDL_strcasecmp(argv[i], "--accel") == 0 && i+1 < argc) {
                accel = atoi(argv[i+1]);
                consumed = 2;
            } else {
                consumed = -1;
            }
        }
        if (consumed < 0) {
            static const char *options[] = { "[--fsaa n]", "[--accel n]", NULL };
            SDLTest_CommonLogUsage(state, argv[0], options);
            quit(1);
        }
        i += consumed;
    }

    /* Set OpenGL parameters */
    state->window_flags |= SDL_WINDOW_OPENGL;
    state->gl_red_size = 5;
    state->gl_green_size = 5;
    state->gl_blue_size = 5;
    state->gl_depth_size = 16;
    state->gl_double_buffer = 1;
    if (fsaa) {
        state->gl_multisamplebuffers = 1;
        state->gl_multisamplesamples = fsaa;
    }
    if (accel >= 0) {
        state->gl_accelerated = accel;
    }

    if (!SDLTest_CommonInit(state)) {
        quit(2);
    }

    /* Create OpenGL context */
    context = SDL_GL_CreateContext(state->windows[0]);
    if (!context) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_GL_CreateContext(): %s\n", SDL_GetError());
        quit(2);
    }

    /* Important: call this *after* creating the context */
    if (LoadContext(&ctx) < 0) {
        SDL_Log("Could not load GL functions\n");
        quit(2);
        return 0;
    }

    if (state->render_flags & SDL_RENDERER_PRESENTVSYNC) {
        /* try late-swap-tearing first. If not supported, try normal vsync. */
        if (SDL_GL_SetSwapInterval(-1) == 0) {
            swap_interval = -1;
        } else {
            SDL_GL_SetSwapInterval(1);
            swap_interval = 1;
        }
    } else {
        SDL_GL_SetSwapInterval(0);  /* disable vsync. */
        swap_interval = 0;
    }

    SDL_GetCurrentDisplayMode(0, &mode);
    SDL_Log("Screen BPP    : %d\n", SDL_BITSPERPIXEL(mode.format));
    SDL_Log("Swap Interval : %d\n", SDL_GL_GetSwapInterval());
    SDL_GetWindowSize(state->windows[0], &dw, &dh);
    SDL_Log("Window Size   : %d,%d\n", dw, dh);
    SDL_GL_GetDrawableSize(state->windows[0], &dw, &dh);
    SDL_Log("Draw Size     : %d,%d\n", dw, dh);
    SDL_Log("\n");
    SDL_Log("Vendor        : %s\n", ctx.glGetString(GL_VENDOR));
    SDL_Log("Renderer      : %s\n", ctx.glGetString(GL_RENDERER));
    SDL_Log("Version       : %s\n", ctx.glGetString(GL_VERSION));
    SDL_Log("Extensions    : %s\n", ctx.glGetString(GL_EXTENSIONS));
    SDL_Log("\n");

    status = SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &value);
    if (!status) {
        SDL_Log("SDL_GL_RED_SIZE: requested %d, got %d\n", 5, value);
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to get SDL_GL_RED_SIZE: %s\n", SDL_GetError());
    }
    status = SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &value);
    if (!status) {
        SDL_Log("SDL_GL_GREEN_SIZE: requested %d, got %d\n", 5, value);
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to get SDL_GL_GREEN_SIZE: %s\n", SDL_GetError());
    }
    status = SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &value);
    if (!status) {
        SDL_Log("SDL_GL_BLUE_SIZE: requested %d, got %d\n", 5, value);
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to get SDL_GL_BLUE_SIZE: %s\n", SDL_GetError());
    }
    status = SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &value);
    if (!status) {
        SDL_Log("SDL_GL_DEPTH_SIZE: requested %d, got %d\n", 16, value);
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to get SDL_GL_DEPTH_SIZE: %s\n", SDL_GetError());
    }
    if (fsaa) {
        status = SDL_GL_GetAttribute(SDL_GL_MULTISAMPLEBUFFERS, &value);
        if (!status) {
            SDL_Log("SDL_GL_MULTISAMPLEBUFFERS: requested 1, got %d\n", value);
        } else {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to get SDL_GL_MULTISAMPLEBUFFERS: %s\n",
                         SDL_GetError());
        }
        status = SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &value);
        if (!status) {
            SDL_Log("SDL_GL_MULTISAMPLESAMPLES: requested %d, got %d\n", fsaa,
                    value);
        } else {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to get SDL_GL_MULTISAMPLESAMPLES: %s\n",
                         SDL_GetError());
        }
    }
    if (accel >= 0) {
        status = SDL_GL_GetAttribute(SDL_GL_ACCELERATED_VISUAL, &value);
        if (!status) {
            SDL_Log("SDL_GL_ACCELERATED_VISUAL: requested %d, got %d\n", accel,
                    value);
        } else {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to get SDL_GL_ACCELERATED_VISUAL: %s\n",
                         SDL_GetError());
        }
    }

    /* Set rendering settings */
    ctx.glMatrixMode(GL_PROJECTION);
    ctx.glLoadIdentity();
    ctx.glOrtho(-2.0, 2.0, -2.0, 2.0, -20.0, 20.0);
    ctx.glMatrixMode(GL_MODELVIEW);
    ctx.glLoadIdentity();
    ctx.glEnable(GL_DEPTH_TEST);
    ctx.glDepthFunc(GL_LESS);
    ctx.glShadeModel(GL_SMOOTH);

    /* Main render loop */
    frames = 0;
    then = SDL_GetTicks();
    done = 0;
    while (!done) {
        SDL_bool update_swap_interval = SDL_FALSE;

        /* Check for events */
        ++frames;
        while (SDL_PollEvent(&event)) {
            SDLTest_CommonEvent(state, &event, &done);
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_o) {
                    swap_interval--;
                    update_swap_interval = SDL_TRUE;
                } else if (event.key.keysym.sym == SDLK_p) {
                    swap_interval++;
                    update_swap_interval = SDL_TRUE;
                }
            }
        }

        if (update_swap_interval) {
            SDL_Log("Swap interval to be set to %d\n", swap_interval);
        }

        for (i = 0; i < state->num_windows; ++i) {
            int w, h;
            if (state->windows[i] == NULL)
                continue;
            SDL_GL_MakeCurrent(state->windows[i], context);
            if (update_swap_interval) {
                SDL_GL_SetSwapInterval(swap_interval);
            }
            SDL_GL_GetDrawableSize(state->windows[i], &w, &h);
            ctx.glViewport(0, 0, w, h);
            Render();
            SDL_GL_SwapWindow(state->windows[i]);
        }
    }

    /* Print out some timing information */
    now = SDL_GetTicks();
    if (now > then) {
        SDL_Log("%2.2f frames per second\n",
                ((double) frames * 1000) / (now - then));
    }
    quit(0);
    return 0;
}


//
//
//static SDL_bool shaders_supported;
//static int      current_shader = 0;
//
//enum {
//    SHADER_COLOR,
//    SHADER_TEXTURE,
//    SHADER_TEXCOORDS,
//    NUM_SHADERS
//};
//
//typedef struct {
//    GLhandleARB program;
//    GLhandleARB vert_shader;
//    GLhandleARB frag_shader;
//    const char *vert_source;
//    const char *frag_source;
//} ShaderData;
//
//
//static ShaderData shaders[NUM_SHADERS] = {
//
//        /* SHADER_COLOR */
//        { 0, 0, 0,
//                /* vertex shader */
//                "varying vec4 v_color;\n"
//                "\n"
//                "void main()\n"
//                "{\n"
//                "    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
//                "    v_color = gl_Color;\n"
//                "}",
//                /* fragment shader */
//                "varying vec4 v_color;\n"
//                "\n"
//                "void main()\n"
//                "{\n"
//                "    gl_FragColor = v_color;\n"
//                "}"
//        },
//
//        /* SHADER_TEXTURE */
//        { 0, 0, 0,
//                /* vertex shader */
//                "varying vec4 v_color;\n"
//                "varying vec2 v_texCoord;\n"
//                "\n"
//                "void main()\n"
//                "{\n"
//                "    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
//                "    v_color = gl_Color;\n"
//                "    v_texCoord = vec2(gl_MultiTexCoord0);\n"
//                "}",
//                /* fragment shader */
//                "varying vec4 v_color;\n"
//                "varying vec2 v_texCoord;\n"
//                "uniform sampler2D tex0;\n"
//                "\n"
//                "void main()\n"
//                "{\n"
//                "    gl_FragColor = texture2D(tex0, v_texCoord) * v_color;\n"
//                "}"
//        },
//
//        /* SHADER_TEXCOORDS */
//        { 0, 0, 0,
//                /* vertex shader */
//                "varying vec2 v_texCoord;\n"
//                "\n"
//                "void main()\n"
//                "{\n"
//                "    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
//                "    v_texCoord = vec2(gl_MultiTexCoord0);\n"
//                "}",
//                /* fragment shader */
//                "varying vec2 v_texCoord;\n"
//                "\n"
//                "void main()\n"
//                "{\n"
//                "    vec4 color;\n"
//                "    vec2 delta;\n"
//                "    float dist;\n"
//                "\n"
//                "    delta = vec2(0.5, 0.5) - v_texCoord;\n"
//                "    dist = dot(delta, delta);\n"
//                "\n"
//                "    color.r = v_texCoord.x;\n"
//                "    color.g = v_texCoord.x * v_texCoord.y;\n"
//                "    color.b = v_texCoord.y;\n"
//                "    color.a = 1.0 - (dist * 4.0);\n"
//                "    gl_FragColor = color;\n"
//                "}"
//        },
//};
//
//
//
//static PFNGLATTACHOBJECTARBPROC glAttachObjectARB;
//static PFNGLCOMPILESHADERARBPROC glCompileShaderARB;
//static PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
//static PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
//static PFNGLDELETEOBJECTARBPROC glDeleteObjectARB;
//static PFNGLGETINFOLOGARBPROC glGetInfoLogARB;
//static PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
//static PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB;
//static PFNGLLINKPROGRAMARBPROC glLinkProgramARB;
//static PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
//static PFNGLUNIFORM1IARBPROC glUniform1iARB;
//static PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;
//
//
//static SDL_bool CompileShader(GLhandleARB shader, const char *source)
//{
//    GLint status;
//
//    glShaderSourceARB(shader, 1, &source, NULL);
//    glCompileShaderARB(shader);
//    glGetObjectParameterivARB(shader, GL_OBJECT_COMPILE_STATUS_ARB, &status);
//    if (status == 0) {
//        GLint length;
//        char *info;
//
//        glGetObjectParameterivARB(shader, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);
//        info = SDL_stack_alloc(char, length+1);
//        glGetInfoLogARB(shader, length, NULL, info);
//        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to compile shader:\n%s\n%s", source, info);
//        SDL_stack_free(info);
//
//        return SDL_FALSE;
//    } else {
//        return SDL_TRUE;
//    }
//}
//
//
//
//static SDL_bool CompileShaderProgram(ShaderData *data)
//{
//    const int num_tmus_bound = 4;
//    int i;
//    GLint location;
//
//    glGetError();
//
//    /* Create one program object to rule them all */
//    data->program = glCreateProgramObjectARB();
//
//    /* Create the vertex shader */
//    data->vert_shader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
//    if (!CompileShader(data->vert_shader, data->vert_source)) {
//        return SDL_FALSE;
//    }
//
//    /* Create the fragment shader */
//    data->frag_shader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
//    if (!CompileShader(data->frag_shader, data->frag_source)) {
//        return SDL_FALSE;
//    }
//
//    /* ... and in the darkness bind them */
//    glAttachObjectARB(data->program, data->vert_shader);
//    glAttachObjectARB(data->program, data->frag_shader);
//    glLinkProgramARB(data->program);
//
//    /* Set up some uniform variables */
//    glUseProgramObjectARB(data->program);
//    for (i = 0; i < num_tmus_bound; ++i) {
//        char tex_name[5];
//        SDL_snprintf(tex_name, SDL_arraysize(tex_name), "tex%d", i);
//        location = glGetUniformLocationARB(data->program, tex_name);
//        if (location >= 0) {
//            glUniform1iARB(location, i);
//        }
//    }
//    glUseProgramObjectARB(0);
//
//    return (glGetError() == GL_NO_ERROR) ? SDL_TRUE : SDL_FALSE;
//}
//
//
//static void DestroyShaderProgram(ShaderData *data)
//{
//    if (shaders_supported) {
//        glDeleteObjectARB(data->vert_shader);
//        glDeleteObjectARB(data->frag_shader);
//        glDeleteObjectARB(data->program);
//    }
//}
//
//
//static SDL_bool InitShaders()
//{
//    int i;
//
//    /* Check for shader support */
//    shaders_supported = SDL_FALSE;
//    if (SDL_GL_ExtensionSupported("GL_ARB_shader_objects") &&
//        SDL_GL_ExtensionSupported("GL_ARB_shading_language_100") &&
//        SDL_GL_ExtensionSupported("GL_ARB_vertex_shader") &&
//        SDL_GL_ExtensionSupported("GL_ARB_fragment_shader")) {
//        glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC) SDL_GL_GetProcAddress("glAttachObjectARB");
//        glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC) SDL_GL_GetProcAddress("glCompileShaderARB");
//        glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC) SDL_GL_GetProcAddress("glCreateProgramObjectARB");
//        glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC) SDL_GL_GetProcAddress("glCreateShaderObjectARB");
//        glDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC) SDL_GL_GetProcAddress("glDeleteObjectARB");
//        glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC) SDL_GL_GetProcAddress("glGetInfoLogARB");
//        glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC) SDL_GL_GetProcAddress("glGetObjectParameterivARB");
//        glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC) SDL_GL_GetProcAddress("glGetUniformLocationARB");
//        glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC) SDL_GL_GetProcAddress("glLinkProgramARB");
//        glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC) SDL_GL_GetProcAddress("glShaderSourceARB");
//        glUniform1iARB = (PFNGLUNIFORM1IARBPROC) SDL_GL_GetProcAddress("glUniform1iARB");
//        glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC) SDL_GL_GetProcAddress("glUseProgramObjectARB");
//        if (glAttachObjectARB &&
//            glCompileShaderARB &&
//            glCreateProgramObjectARB &&
//            glCreateShaderObjectARB &&
//            glDeleteObjectARB &&
//            glGetInfoLogARB &&
//            glGetObjectParameterivARB &&
//            glGetUniformLocationARB &&
//            glLinkProgramARB &&
//            glShaderSourceARB &&
//            glUniform1iARB &&
//            glUseProgramObjectARB) {
//            shaders_supported = SDL_TRUE;
//        }
//    }
//
//    if (!shaders_supported) {
//        return SDL_FALSE;
//    }
//
//    /* Compile all the shaders */
//    for (i = 0; i < NUM_SHADERS; ++i) {
//        if (!CompileShaderProgram(&shaders[i])) {
//            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to compile shader!\n");
//            return SDL_FALSE;
//        }
//    }
//
//    /* We're done! */
//    return SDL_TRUE;
//}
//
//
//static void QuitShaders()
//{
//    int i;
//
//    for (i = 0; i < NUM_SHADERS; ++i) {
//        DestroyShaderProgram(&shaders[i]);
//    }
//}
//
//
///* Quick utility function for texture creation */
//static int
//power_of_two(int input)
//{
//    int value = 1;
//
//    while (value < input) {
//        value <<= 1;
//    }
//    return value;
//}
//
//
//GLuint
//SDL_GL_LoadTexture(SDL_Surface * surface, GLfloat * texcoord)
//{
//    GLuint texture;
//    int w, h;
//    SDL_Surface *image;
//    SDL_Rect area;
//    SDL_BlendMode saved_mode;
//
//    /* Use the surface width and height expanded to powers of 2 */
//    w = power_of_two(surface->w);
//    h = power_of_two(surface->h);
//    texcoord[0] = 0.0f;         /* Min X */
//    texcoord[1] = 0.0f;         /* Min Y */
//    texcoord[2] = (GLfloat) surface->w / w;     /* Max X */
//    texcoord[3] = (GLfloat) surface->h / h;     /* Max Y */
//
//    image = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32,
//#if SDL_BYTEORDER == SDL_LIL_ENDIAN     /* OpenGL RGBA masks */
//                                 0x000000FF,
//                                 0x0000FF00, 0x00FF0000, 0xFF000000
//#else
//    0xFF000000,
//                                 0x00FF0000, 0x0000FF00, 0x000000FF
//#endif
//    );
//    if (image == NULL) {
//        return 0;
//    }
//
//    /* Save the alpha blending attributes */
//    SDL_GetSurfaceBlendMode(surface, &saved_mode);
//    SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);
//
//    /* Copy the surface into the GL texture image */
//    area.x = 0;
//    area.y = 0;
//    area.w = surface->w;
//    area.h = surface->h;
//    SDL_BlitSurface(surface, &area, image, &area);
//
//    /* Restore the alpha blending attributes */
//    SDL_SetSurfaceBlendMode(surface, saved_mode);
//
//    /* Create an OpenGL texture for the image */
//    glGenTextures(1, &texture);
//    glBindTexture(GL_TEXTURE_2D, texture);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexImage2D(GL_TEXTURE_2D,
//                 0,
//                 GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
//    SDL_FreeSurface(image);     /* No longer needed */
//
//    return texture;
//}
//
//
///* A general OpenGL initialization function.    Sets all of the initial parameters. */
//void InitGL(int Width, int Height)                    /* We call this right after our OpenGL window is created. */
//{
//    GLdouble aspect;
//
//    glViewport(0, 0, Width, Height);
//    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);        /* This Will Clear The Background Color To Black */
//    glClearDepth(1.0);                /* Enables Clearing Of The Depth Buffer */
//    glDepthFunc(GL_LESS);                /* The Type Of Depth Test To Do */
//    glEnable(GL_DEPTH_TEST);            /* Enables Depth Testing */
//    glShadeModel(GL_SMOOTH);            /* Enables Smooth Color Shading */
//
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();                /* Reset The Projection Matrix */
//
//    aspect = (GLdouble)Width / Height;
//    glOrtho(-3.0, 3.0, -3.0 / aspect, 3.0 / aspect, 0.0, 1.0);
//
//    glMatrixMode(GL_MODELVIEW);
//}
//
///* The main drawing function. */
//
//
///* The main drawing function. */
//void DrawGLScene(SDL_Window *window, GLuint texture, GLfloat * texcoord)
//{
//    /* Texture coordinate lookup, to make it simple */
//    enum {
//        MINX,
//        MINY,
//        MAXX,
//        MAXY
//    };
//
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        /* Clear The Screen And The Depth Buffer */
//    glLoadIdentity();                /* Reset The View */
//
//    glTranslatef(-1.5f,0.0f,0.0f);        /* Move Left 1.5 Units */
//
//    /* draw a triangle (in smooth coloring mode) */
//    glBegin(GL_POLYGON);                /* start drawing a polygon */
//    glColor3f(1.0f,0.0f,0.0f);            /* Set The Color To Red */
//    glVertex3f( 0.0f, 1.0f, 0.0f);        /* Top */
//    glColor3f(0.0f,1.0f,0.0f);            /* Set The Color To Green */
//    glVertex3f( 1.0f,-1.0f, 0.0f);        /* Bottom Right */
//    glColor3f(0.0f,0.0f,1.0f);            /* Set The Color To Blue */
//    glVertex3f(-1.0f,-1.0f, 0.0f);        /* Bottom Left */
//    glEnd();                    /* we're done with the polygon (smooth color interpolation) */
//
//    glTranslatef(3.0f,0.0f,0.0f);         /* Move Right 3 Units */
//
//    /* Enable blending */
//    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//    /* draw a textured square (quadrilateral) */
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, texture);
//    glColor3f(1.0f,1.0f,1.0f);
//    if (shaders_supported) {
//        glUseProgramObjectARB(shaders[current_shader].program);
//    }
//
//    glBegin(GL_QUADS);                /* start drawing a polygon (4 sided) */
//    glTexCoord2f(texcoord[MINX], texcoord[MINY]);
//    glVertex3f(-1.0f, 1.0f, 0.0f);        /* Top Left */
//    glTexCoord2f(texcoord[MAXX], texcoord[MINY]);
//    glVertex3f( 1.0f, 1.0f, 0.0f);        /* Top Right */
//    glTexCoord2f(texcoord[MAXX], texcoord[MAXY]);
//    glVertex3f( 1.0f,-1.0f, 0.0f);        /* Bottom Right */
//    glTexCoord2f(texcoord[MINX], texcoord[MAXY]);
//    glVertex3f(-1.0f,-1.0f, 0.0f);        /* Bottom Left */
//    glEnd();                    /* done with the polygon */
//
//    if (shaders_supported) {
//        glUseProgramObjectARB(0);
//    }
//    glDisable(GL_TEXTURE_2D);
//
//    /* swap buffers to display, since we're double buffered. */
//    SDL_GL_SwapWindow(window);
//}
//
//int main(int argc, char **argv)
//{
//    int done;
//    SDL_Window *window;
//    SDL_Surface *surface;
//    GLuint texture;
//    GLfloat texcoords[4];
//
//    /* Enable standard application logging */
//    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
//
//    /* Initialize SDL for video output */
//    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
//        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize SDL: %s\n", SDL_GetError());
//        exit(1);
//    }
//
//    /* Create a 640x480 OpenGL screen */
//    window = SDL_CreateWindow( "Shader Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL );
//    if ( !window ) {
//        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create OpenGL window: %s\n", SDL_GetError());
//        SDL_Quit();
//        exit(2);
//    }
//
//    if ( !SDL_GL_CreateContext(window)) {
//        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create OpenGL context: %s\n", SDL_GetError());
//        SDL_Quit();
//        exit(2);
//    }
//
//    surface = SDL_LoadBMP(path.c_str());
//    if ( ! surface ) {
//        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load icon.bmp: %s\n", SDL_GetError());
//        SDL_Quit();
//        exit(3);
//    }
//    texture = SDL_GL_LoadTexture(surface, texcoords);
//    SDL_FreeSurface(surface);
//
//    /* Loop, drawing and checking events */
//    InitGL(640, 480);
//    if (InitShaders()) {
//        SDL_Log("Shaders supported, press SPACE to cycle them.\n");
//    } else {
//        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Shaders not supported!\n");
//    }
//    done = 0;
//    while ( ! done ) {
//        DrawGLScene(window, texture, texcoords);
//
//        /* This could go in a separate function */
//        { SDL_Event event;
//            while ( SDL_PollEvent(&event) ) {
//                if ( event.type == SDL_QUIT ) {
//                    done = 1;
//                }
//                if ( event.type == SDL_KEYDOWN ) {
//                    if ( event.key.keysym.sym == SDLK_SPACE ) {
//                        current_shader = (current_shader + 1) % NUM_SHADERS;
//                    }
//                    if ( event.key.keysym.sym == SDLK_ESCAPE ) {
//                        done = 1;
//                    }
//                }
//            }
//        }
//    }
//    QuitShaders();
//    SDL_Quit();
//    return 1;
//}
