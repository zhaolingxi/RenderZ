#####################################################################################
# description: 数据库处理框架
# author: zhaolingxi
#####################################################################################
cmake_minimum_required (VERSION 3.8)
project(zdatabase)
if(MSVC)
	add_definitions(-D_ZDATABASE_EXPORTS -D_MSVC)
endif()
include_directories(../zutils ../zutils/include)
include_directories(../zcore ../zcore/include)
include_directories(include)
include_directories(sqlite3)

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

