cmake_minimum_required (VERSION 3.8)
project(zutils)

include_directories(include include/encode include/file)
#三方库头文件
include_directories(${triLibraryPath}/prebuildlib/${TARGET_DIRNAME}/jsoncpp/include/json)

#三方库依赖
link_directories(${triLibraryPath}/prebuildlib/${TARGET_DIRNAME}/jsoncpp/lib)


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


add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} "include/zplog_wrapper.h")
file(GLOB JSONCPP_LIBS "${triLibraryPath}/prebuildlib/${TARGET_DIRNAME}/jsoncpp/lib/*jsoncpp*")
message(STATUS "Found jsoncpp libs: ${JSONCPP_LIBS}")
if(JSONCPP_LIBS)
    target_link_libraries(${PROJECT_NAME} ${JSONCPP_LIBS})
else()
    message(FATAL_ERROR "jsoncpp library not found")
endif()
#target_link_libraries(${PROJECT_NAME} jsoncpp)

if(GNUC)
	target_link_libraries(${PROJECT_NAME} pthread rt dl uuid)
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath='$ORIGIN:$ORIGIN/lib'")
endif()




