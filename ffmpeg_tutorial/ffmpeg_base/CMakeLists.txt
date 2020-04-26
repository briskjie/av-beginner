file(GLOB_RECURSE FFMPEG_BASE_HEADER ./*.h)
file(GLOB_RECURSE FFMPEG_BASE_SOURCE ./*.cpp)

add_library(ffmpeg_base SHARED
        ${FFMPEG_BASE_HEADER} ${FFMPEG_BASE_SOURCE})

target_link_sdl(base)

set_target_properties(ffmpeg_base PROPERTIES LINKER_LANGUAGE CXX)
