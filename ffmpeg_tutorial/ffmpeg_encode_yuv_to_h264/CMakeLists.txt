add_executable(ffmpeg_encode_yuv_to_h264
        ffmpeg_yuv_to_h264.cpp
        fFmpegYuvToH264.cpp
        )

link_directories(./)

target_link_sdl(ffmpeg_encode_yuv_to_h264)
target_link_ffmpeg(ffmpeg_encode_yuv_to_h264)

target_link_ffmpeg_base(ffmpeg_encode_yuv_to_h264)
