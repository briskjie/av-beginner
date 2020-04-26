add_executable(ffmpeg_03_open_codec ffmpeg_open_codec.cpp player.cpp)

link_directories(./)

target_link_ffmpeg(ffmpeg_03_open_codec)

target_link_ffmpeg_base(ffmpeg_03_open_codec)
