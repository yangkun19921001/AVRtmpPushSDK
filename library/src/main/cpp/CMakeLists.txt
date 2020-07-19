cmake_minimum_required(VERSION 3.4.1)

#配置 librtmp 库的路径
set(LIBRTMP_ROOT_PATH ${CMAKE_SOURCE_DIR}/librtmp)
set(JNI_ROOT_PATH ${CMAKE_SOURCE_DIR}/jni)
set(PUSH_ROOT_PATH ${CMAKE_SOURCE_DIR}/push)
set(COMMON_ROOT_PATH ${CMAKE_SOURCE_DIR}/common)
set(CALLBACK_ROOT_PATH ${CMAKE_SOURCE_DIR}/callback)

#librtmp H 文件
include_directories(${LIBRTMP_ROOT_PATH}/include)
#push H 文件
include_directories(${PUSH_ROOT_PATH})
#JNI H 文件
include_directories(${JNI_ROOT_PATH})
#核心库 H 文件
include_directories(${COMMON_ROOT_PATH})
#Java callback
include_directories(${CALLBACK_ROOT_PATH})


#jni cpp 文件
FILE(GLOB JNI_ALL_CPP ${JNI_ROOT_PATH}/*.cpp)
#push cpp 文件
FILE(GLOB PUSH_ALL_CPP ${PUSH_ROOT_PATH}/*.cpp)
#核心库 cpp 文件
FILE(GLOB COMMON_ALL_CPP ${COMMON_ROOT_PATH}/*.cpp)
#callback
FILE(GLOB CALLBACK_ALL_CPP ${CALLBACK_ROOT_PATH}/*.cpp)


add_library(
        AVRtmpPush
        SHARED
        ${JNI_ALL_CPP}
        ${PUSH_ALL_CPP}
        ${COMMON_ALL_CPP}
        ${CALLBACK_ALL_CPP}
)

#librtmp 静态库
add_library(rtmp STATIC IMPORTED)
set_target_properties(rtmp PROPERTIES IMPORTED_LOCATION ${LIBRTMP_ROOT_PATH}/libs/${CMAKE_ANDROID_ARCH_ABI}/librtmp.a)




find_library(log-lib log)

target_link_libraries(
        AVRtmpPush
        rtmp
        ${log-lib}
)