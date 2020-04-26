add_executable(ffmpeg_04_decode_video ffmpeg_decode_video.cpp decoder.cpp)

link_directories(./)

target_link_ffmpeg(ffmpeg_04_decode_video)

target_link_ffmpeg_base(ffmpeg_04_decode_video)
