cmake_minimum_required(VERSION 3.17)
project(CasinoGame)
set(CMAKE_CXX_STANDARD 14)
# Paths to SDL2, SDL2_image, SDL2_ttf, and SDL2_mixer
set(SDL2_INCLUDE_DIR C:/libs/SDL2/include)
set(SDL2_LIB_DIR C:/libs/SDL2/lib/x64)
set(SDL2_MIXER_INCLUDE_DIR C:/libs/SDL_mixer/include)
set(SDL2_MIXER_LIB_DIR C:/libs/SDL_mixer/lib/x64)
# Include directories
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR})
# Link directories
link_directories(${SDL2_LIB_DIR} ${SDL2_MIXER_LIB_DIR})
# Add executable
add_executable(CasinoGame main.cpp)
# Link SDL2, SDL2_image, SDL2_ttf, and SDL2_mixer libraries
target_link_libraries(CasinoGame SDL2main SDL2 SDL2_image SDL2_ttf SDL2_mixer)

add_custom_command(
        TARGET CasinoGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:CasinoGame>/assets
)