add_executable(ffmpeg_encode_yuv_to_mp4
        ffmpeg_yuv_to_mp4.cpp
        FFmpegYuvToMp4.cpp
        )

link_directories(./)

target_link_sdl(ffmpeg_encode_yuv_to_mp4)
target_link_ffmpeg(ffmpeg_encode_yuv_to_mp4)

target_link_ffmpeg_base(ffmpeg_encode_yuv_to_mp4)
