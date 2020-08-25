add_executable(ffmpeg_encode_yuv_to_h264_new
        YuvConvert.cpp
        main.cpp
        IConvert.cpp
        )

link_directories(./)

target_link_sdl(ffmpeg_encode_yuv_to_h264_new)
target_link_ffmpeg(ffmpeg_encode_yuv_to_h264_new)

target_link_ffmpeg_base(ffmpeg_encode_yuv_to_h264_new)
