set(list ffplay.c config.h cmdutils.h cmdutils.c)

add_executable(ffmpeg_00_run_ffplay ${list})

target_link_sdl(ffmpeg_00_run_ffplay)
target_link_ffmpeg(ffmpeg_00_run_ffplay)

include_directories(./)

init_definition()