#####################################################################################
# description: 应用程序基础框架与服务，包括消息中心、模块管理器
#####################################################################################
cmake_minimum_required (VERSION 3.8)
project(zutils)

include_directories(include include/encode include/json include/file)

#三方库头文件
set (TRILIBRARY_PATHS ../../triLibrary)
include_directories(${TRILIBRARY_PATHS}/jsoncpp/include )
if(MSVC)
	add_definitions(-D _ZUTILS_EXPORTS)
endif()

# 项目头文件
file(GLOB_RECURSE H_HEADER_FILES "include/*.h")
set(HEADER_FILES ${HEADER_FILES} ${H_HEADER_FILES})
file(GLOB_RECURSE H_HEADER_FILES "src/*.h")
set(HEADER_FILES ${HEADER_FILES} ${H_HEADER_FILES})
file(GLOB_RECURSE HPP_HEADER_FILES "include/*.hpp")
set(HEADER_FILES ${HEADER_FILES} ${HPP_HEADER_FILES})

# 项目源文件
file(GLOB_RECURSE CPP_SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE C_SOURCE_FILES "src/*.c")
set(SOURCE_FILES ${SOURCE_FILES} ${CPP_SOURCE_FILES} ${C_SOURCE_FILES})



add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

find_library(../../triLibrary/jsoncpp/${TARGET_DIRNAME}/lib)
add_executable(jsoncpp)
target_link_libraries(../../triLibrary/jsoncpp/${TARGET_DIRNAME}/lib jsoncpp)
if(GNUC)
	# 需要安装uuid依赖,sudo apt-get install uuid-dev
	target_link_libraries(${PROJECT_NAME} pthread rt dl uuid)
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath='$ORIGIN:$ORIGIN/lib'")
endif()


