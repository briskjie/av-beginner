add_executable(ffmpeg_encode_yuv_to_mp4_new
        main.cpp
        )

link_directories(./)

target_link_sdl(ffmpeg_encode_yuv_to_mp4_new)
target_link_ffmpeg(ffmpeg_encode_yuv_to_mp4_new)

target_link_ffmpeg_base(ffmpeg_encode_yuv_to_mp4_new)
