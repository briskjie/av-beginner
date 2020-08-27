add_executable(ffmpeg_seek
        main.cpp
        Player.cpp
        )

link_directories(./include)

target_link_sdl(ffmpeg_seek)
target_link_ffmpeg(ffmpeg_seek)

target_link_ffmpeg_base(ffmpeg_seek)
