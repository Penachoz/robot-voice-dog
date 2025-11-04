cmake_minimum_required(VERSION 3.16)
project(robot_voice_dog LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/whisper.cpp)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PORTAUDIO REQUIRED portaudio-2.0)

add_library(audio modules/audio/audio_capture.cpp)
target_include_directories(audio PUBLIC modules/audio ${PORTAUDIO_INCLUDE_DIRS})
target_link_libraries(audio PUBLIC ${PORTAUDIO_LIBRARIES})

add_library(stt modules/stt/whisper_wrapper.cpp)
target_include_directories(stt PUBLIC modules/stt external/whisper.cpp)
target_link_libraries(stt PUBLIC whisper)

add_library(command modules/command/command_router.cpp)
target_include_directories(command PUBLIC modules/command)

add_executable(voice_controller src/main.cpp)
target_link_libraries(voice_controller PRIVATE audio stt command)

set_target_properties(voice_controller PROPERTIES BUILD_RPATH_USE_ORIGIN ON)
