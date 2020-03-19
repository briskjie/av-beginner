message(${CMAKE_CURRENT_SOURCE_DIR})

macro(init_path)
    message("init library path")
    set(base_dir "${CMAKE_SOURCE_DIR}/base")
    set(sdl_dir "/usr/local/Cellar/sdl2/2.0.10")
    set(sdl_image_dir "/usr/local/Cellar/sdl2_image/2.0.5")
    set(sound_touch_dir "/usr/local/Cellar/sound-touch/2.1.2")
    set(glew_dir "/usr/local/Cellar/glew/2.1.0_1")
    set(glfw_dir "/usr/local/Cellar/glfw/3.3.2")
    set(glm_dir "/usr/local/Cellar/glm/0.9.9.5")
endmacro()

# ----------------  link library  ----------------

macro(link_base)
    message("link base")
    include_directories(${base_dir}/gl-base/include)
    include_directories(${base_dir}/common/)
endmacro()

macro(link_sdl)
    message("link sdl library and path is ${sdl_dir} ")
    include_directories(${sdl_dir}/include/SDL2)
    link_directories(${sdl_dir}/lib/)
endmacro()

macro(link_sdl_image)
    message("link sdl image library and path is ${sdl_image_dir}")
    include_directories(${sdl_image_dir}/include/SDL2)
    link_directories(${sdl_image_dir}/lib)
endmacro()

macro(link_sound_touch)
    message("link sound touch library and path is ${sound_touch_dir}")
    include_directories(${sound_touch_dir}/include/soundtouch)
    link_directories(${sound_touch_dir}/lib)
endmacro()

macro(link_glew)
    message("link glew library and path is ${glew_dir}")
    include_directories(${glew_dir}/include/)
    link_directories(${glew_dir}/lib/)
endmacro()

macro(link_glfw)
    message("link glfw library and path is ${glfw_dir}")
    include_directories(${glfw_dir}/include/)
    link_directories(${glfw_dir}/lib/)
endmacro()

macro(link_glm)
    message("link glm library and path is ${glm_dir}")
    include_directories(${glm_dir}/include/)
    link_directories(${glm_dir}/lib/)
endmacro()


# ----------------  target link library  ----------------

macro(target_link_base name)
    message("${name} link base library")
    target_link_libraries(${name}
            base)
endmacro()


macro(target_link_sdl name)
    message("${name} link sdl library")
    target_link_libraries(${name}
            SDL2
            SDL2_test
            SDL2main)
endmacro()

macro(target_link_sdl_image name)
    message("${name} link sdl image")
    target_link_libraries(${name}
            SDL2_image
            )
endmacro()

macro(target_link_sound_touch name)
    message("${name} link sound touch")
##   target_link_libraries 直接链接 SoundTouch 出问题，加一个 find_library 就好了
#    find_library(soundTouch SoundTouch)
    target_link_libraries(${name} SoundTouch)
endmacro()

macro(target_link_glew name)
    message("${name} link glew")
    target_link_libraries(${name}
            GLEW
            )
endmacro()

macro(target_link_glfw name)
    message("${name} link glfw")
    target_link_libraries(${name}
            glfw
            )
endmacro()



# 需要链接系统的 OpenGL 库，不然 glew 使用会报错 symbol(s) not found for architecture x86_64
macro(target_link_opengl name)
    message("${name} link OpenGL")
    if (APPLE)
        target_link_libraries(${name} "-framework OpenGL")
    endif ()
endmacro()

# ----------------  init definition  ----------------

macro(init_definition)
    add_definitions(-DPROJECT_DIR_PATH="${CMAKE_SOURCE_DIR}")
endmacro()