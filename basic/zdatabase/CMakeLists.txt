﻿cmake_minimum_required (VERSION 3.8)
project(zdatabase)

include_directories(../zutils ../zutils/include)
include_directories(../zcore ../zcore/include)
include_directories(sqlite3)

include_directories(include src)

if(MSVC)
	add_definitions(-D_ZDATABASE_EXPORTS)
endif()

file(GLOB HEADER_FILES ${HEADER_FILES} "sqlite3/*.h")  # sqlite3头文件
file(GLOB SOURCE_FILES ${SOURCE_FILES} "sqlite3/*.c")  # sqlite3源文件
file(GLOB HEADER_FILES ${HEADER_FILES} "include/*.h")  # 头文件
file(GLOB SOURCE_FILES ${SOURCE_FILES} "src/*.cpp")    # 源文件

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} zutils zcore)
if(GNUC)
	target_link_libraries(${PROJECT_NAME} pthread rt dl)
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath='$ORIGIN:$ORIGIN/lib'")
endif()

