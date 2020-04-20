include_directories(${CMAKE_CURRENT_SOURCE_DIR}/gl_base/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/gl_base)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/stb)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/shader_toy)
include_directories(${CMAKE_SOURCE_DIR}/glad_gl_3_3/include)


file(GLOB_RECURSE COMMON_BASE_HEADER ./common/*.h)
file(GLOB_RECURSE COMMON_BASE_SOURCE ./common/*.cpp)

file(GLOB_RECURSE GL_BASE_HEADER gl_base/*.h)
file(GLOB_RECURSE GL_BASE_SOURCE gl_base/*.cpp)

file(GLOB_RECURSE DISPLAY_HEADER display/*.h)
file(GLOB_RECURSE DISPLAY_SOURCE display/*.cpp)

file(GLOB_RECURSE SHADER_TOY_HEADER shader_toy/*.h)
file(GLOB_RECURSE SHADER_TOY_SOURCE shader_toy/*.cpp)

add_library(base SHARED
        ${COMMON_BASE_HEADER} ${COMMON_BASE_SOURCE}
        ${GL_BASE_HEADER} ${GL_BASE_SOURCE}
        ${DISPLAY_HEADER} ${DISPLAY_SOURCE}
        ${SHADER_TOY_HEADER} ${SHADER_TOY_SOURCE}
        ${CMAKE_SOURCE_DIR}/glad_gl_3_3/src/glad.c)

target_link_sdl(base)

set_target_properties(base PROPERTIES LINKER_LANGUAGE CXX)