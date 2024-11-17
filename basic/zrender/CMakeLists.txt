cmake_minimum_required (VERSION 3.8)
project(zrender)

# 头文件包含目录
include_directories(../zutils  ../zutils/include)
include_directories(../zcore  ../zcore/include)
include_directories(../zmath  ../zmath/include)
include_directories(include src)
include_directories(include/model_loader src/model_loader)
include_directories(include/scene src/scene)

if(MSVC)
	add_definitions(-D_ZRENDER_EXPORTS)
endif()

# 项目头文件
file(GLOB_RECURSE H_HEADER_FILES "include/*.h")
file(GLOB_RECURSE HPP_HEADER_FILES "include/*.hpp")
set(HEADER_FILES ${HEADER_FILES} ${H_HEADER_FILES} ${HPP_HEADER_FILES})

# 项目源文件
file(GLOB_RECURSE CPP_SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE C_SOURCE_FILES "src/*.c")
set(SOURCE_FILES ${SOURCE_FILES} ${CPP_SOURCE_FILES} ${C_SOURCE_FILES})

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} zutils zmath zcore)
if(GNUC)
	target_link_libraries(${PROJECT_NAME} pthread rt dl)
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath='$ORIGIN:$ORIGIN/lib'")
endif()