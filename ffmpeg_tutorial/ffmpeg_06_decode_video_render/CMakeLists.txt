add_executable(ffmpeg_06_decode_video_render
        ffmpeg_decode_video_queue.cpp
        decoder.cpp
        display_.cpp
        SDLWrapper.cpp
        )

link_directories(./)

target_link_sdl(ffmpeg_06_decode_video_render)
target_link_ffmpeg(ffmpeg_06_decode_video_render)

target_link_ffmpeg_base(ffmpeg_06_decode_video_render)
