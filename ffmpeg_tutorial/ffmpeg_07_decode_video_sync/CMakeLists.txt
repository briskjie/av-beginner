add_executable(ffmpeg_07_decode_video_sync
        ffmpeg_decode_video_sync.cpp
        decoder.cpp
        SDLWrapper.cpp
        )

link_directories(./)

target_link_sdl(ffmpeg_07_decode_video_sync)
target_link_ffmpeg(ffmpeg_07_decode_video_sync)

target_link_ffmpeg_base(ffmpeg_07_decode_video_sync)
