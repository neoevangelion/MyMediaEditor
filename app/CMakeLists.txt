cmake_minimum_required(VERSION 3.4.1)

add_library(
        player-lib
        SHARED
        src/main/cpp/player.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/jniLibs/include)

add_library(
        avcodec-lib
        SHARED
        IMPORTED
)

set_target_properties(
        avcodec-lib
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/jniLibs/${ANDROID_ABI}/libavcodec.so
)

add_library(
        avdevice-lib
        SHARED
        IMPORTED
)

set_target_properties(
        avdevice-lib
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/jniLibs/${ANDROID_ABI}/libavdevice.so
)

add_library(
        avfilter-lib
        SHARED
        IMPORTED
)

set_target_properties(
        avfilter-lib
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/jniLibs/${ANDROID_ABI}/libavfilter.so
)
add_library(
        avformat-lib
        SHARED
        IMPORTED
)

set_target_properties(
        avformat-lib
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/jniLibs/${ANDROID_ABI}/libavformat.so
)

add_library(
        avutil-lib
        SHARED
        IMPORTED
)

set_target_properties(
        avutil-lib
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/jniLibs/${ANDROID_ABI}/libavutil.so
)

add_library(
        swscale-lib
        SHARED
        IMPORTED
)

set_target_properties(
        swscale-lib
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/jniLibs/${ANDROID_ABI}/libswscale.so
)

add_library(
        swresample-lib
        SHARED
        IMPORTED
)
set_target_properties(
        swresample-lib
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/jniLibs/${ANDROID_ABI}/libswresample.so
)

target_link_libraries(
        player-lib
        log
        android
        avcodec-lib
        avdevice-lib
        avfilter-lib
        avformat-lib
        avutil-lib
        swscale-lib
        swresample-lib
)
