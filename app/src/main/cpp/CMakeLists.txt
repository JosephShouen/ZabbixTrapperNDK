cmake_minimum_required(VERSION 3.22.1)
project("zabbixtrapperndk")

# флаги скорее всего нужны для shared lib
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

#основная либа, вызываемая в kotlin, temp
add_library(${CMAKE_PROJECT_NAME} SHARED
        zabbixnativecontroller.cpp)

#static либы boost
add_library( boost_chrono STATIC IMPORTED)
set_target_properties(boost_chrono PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/static/libboost_chrono.a)
add_library( boost_atomic STATIC IMPORTED)
set_target_properties(boost_atomic PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/static/libboost_atomic.a)

#trapper zabbix lib
add_library(zabbixtrapper STATIC
        zabbixtrapper.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../include/zabbixtrapper.h
)
target_include_directories(zabbixtrapper PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(zabbixtrapper boost_atomic boost_chrono)

#include and final build
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries( # Specifies the target library.
        ${CMAKE_PROJECT_NAME}
        android
        log
        zabbixtrapper
)
