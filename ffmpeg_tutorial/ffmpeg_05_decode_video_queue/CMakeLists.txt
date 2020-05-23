add_executable(ffmpeg_05_decode_video_queue
        ffmpeg_decode_video_queue.cpp
        decoder.cpp
        )

link_directories(./)

target_link_ffmpeg(ffmpeg_05_decode_video_queue)

target_link_ffmpeg_base(ffmpeg_05_decode_video_queue)
