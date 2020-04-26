add_executable(ffmpeg_02_file_info ffmpeg_file_info.cpp fileInfo.h fileInfo.cpp)

link_directories(./)

target_link_ffmpeg(ffmpeg_02_file_info)

target_link_ffmpeg_base(ffmpeg_02_file_info)
