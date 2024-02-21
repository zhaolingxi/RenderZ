#####################################################################################
# description: 可视化组件管理工具
#####################################################################################
cmake_minimum_required (VERSION 3.14)
project ("RenderZ")

# 默认生成linux64位下的debug版
if (NOT PLATFORM_NAME)
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(PLATFORM_NAME "LINUX64")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        set(PLATFORM_NAME "WIN64")
    else()
        message(FATAL "Unsupported current operating system.")
    endif()
endif()
if (NOT BUILD_TEST)
        set(BUILD_TEST "True")
endif()
if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Debug")
endif()

# cmake模块路径
set(CMAKE_MODULE_PATH
	${CMAKE_CURRENT_SOURCE_DIR}/makefile/cmake
	${CMAKE_CURRENT_SOURCE_DIR}/makefile/cmake/platforms/
)
# 编译默认配置和编译路径
include(build_utils)
config_target_path(${CMAKE_CURRENT_SOURCE_DIR})
config_build_path(${CMAKE_CURRENT_SOURCE_DIR})

# 公共编译选项
include(common)
# 不同平台的编译选项
if (MSVC)
	message("Info: set windows ${PROJECT_NAME} compile flag, MSVC: ${MSVC}")
	include(windows)
elseif(GNUC)
	message("Info: set linux ${PROJECT_NAME} compile flag, GNUC: ${GNUC}")
	include(linux)
endif()
message("---compile ${PROJECT_NAME} Platform: ${PLATFORM_NAME}, Debug: ${DEBUG}, CXX: ${CMAKE_CXX_COMPILER}")

# 第3方库
add_subdirectory ("basic") # 基础库
#add_subdirectory ("logic") # 业务插件模块
add_subdirectory ("pages") # 功能页面插件
#add_subdirectory ("service") # 服务模块
add_subdirectory ("application")   # 应用程序
add_subdirectory ("triLibrary") # 三方库
