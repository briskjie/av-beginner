file(GLOB_RECURSE COMMON_BASE_HEADER ./common/*.h)
file(GLOB_RECURSE COMMON_BASE_SOURCE ./common/*.cpp)

file(GLOB_RECURSE GL_BASE_HEADER ./gl-base/*.h)
file(GLOB_RECURSE GL_BASE_SOURCE ./gl-base/*.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/gl-base/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/common)


set(glew_dir "/usr/local/Cellar/glew/2.1.0_1")

include_directories(${glew_dir}/include/)
link_directories(${glew_dir}/lib/)


add_library(base SHARED
        ${COMMON_BASE_HEADER} ${COMMON_BASE_SOURCE}
        ${GL_BASE_HEADER} ${GL_BASE_SOURCE})

target_link_glew(base)

set_target_properties(base PROPERTIES LINKER_LANGUAGE CXX)