include_directories(./ffmpeg_base)

macro(target_link_ffmpeg_base name)
    message("${name} link ffmpeg_base")
    target_link_libraries(${name}
            ffmpeg_base
            )
endmacro()

add_subdirectory(ffmpeg_base)
add_subdirectory(ffmpeg_00_run_ffplay)
add_subdirectory(ffmpeg_01_print_log)
add_subdirectory(ffmpeg_02_file_info)
add_subdirectory(ffmpeg_03_open_codec)
add_subdirectory(ffmpeg_04_decode_video)
add_subdirectory(ffmpeg_05_decode_video_queue)
add_subdirectory(ffmpeg_06_decode_video_render)
add_subdirectory(ffmpeg_07_decode_video_sync)
add_subdirectory(ffmpeg_encode_yuv_to_h264)
add_subdirectory(ffmpeg_encode_yuv_to_h264_new)
add_subdirectory(ffmpeg_encode_yuv_to_mp4)
add_subdirectory(ffmpeg_encode_yuv_to_mp4_new)

