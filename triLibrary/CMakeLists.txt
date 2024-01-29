cmake_minimum_required(VERSION 3.8)
project(triLibrary)
# 默认生成linux64位下的debug版
if (NOT PLATFORM_NAME)
        set(PLATFORM_NAME "LINUX64")
endif()
if (NOT BUILD_TEST)
        set(BUILD_TEST "True")
endif()
if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Debug")
endif()

# cmake模块路径
set(CMAKE_MODULE_PATH
	${CMAKE_CURRENT_SOURCE_DIR}/../makefile/cmake
	${CMAKE_CURRENT_SOURCE_DIR}/../makefile/cmake/platforms/
)
# 编译默认配置和编译路径
include(build_utils) 
config_build_path(${CMAKE_CURRENT_SOURCE_DIR})
config_target_dirname()

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

# 编译结果保存目录
set(3RDPARTY_TARGET_DIR ${CMAKE_CURRENT_SOURCE_DIR}/prebuild/${TARGET_DIRNAME})
# 编译配置
if(DEBUG)
    set(3RDPARTY_BUILD_CONFIG "Debug")
else()
    set(3RDPARTY_BUILD_CONFIG "Release")
endif()
message("Info: end config 3rdparty target dir: ${3RDPARTY_TARGET_DIR}, build config: ${3RDPARTY_BUILD_CONFIG}")

# 执行3方库编译脚本
macro(run_3rdparty_build_script _script_path _install_dir)
    message("Info: run ${_script_path} it will cost long time please wait....")
    if(MSVC)
        execute_process(COMMAND cmd /c ${_script_path} "${CMAKE_COMMAND}" "${3RDPARTY_BUILD_CONFIG}" "${_install_dir}" 
                        RESULT_VARIABLE CMD_RESULT
                        OUTPUT_VARIABLE CMD_OUTPUT
                        ERROR_VARIABLE CMD_ERROR
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/scripts)
    else()
        execute_process(COMMAND sh ${_script_path} "${CMAKE_COMMAND}" "${3RDPARTY_BUILD_CONFIG}" "${_install_dir}" 
                        RESULT_VARIABLE CMD_RESULT
                        OUTPUT_VARIABLE CMD_OUTPUT
                        ERROR_VARIABLE CMD_ERROR
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/scripts)
    endif()
    message(STATUS "${_script_path} output: ${CMD_OUTPUT}")
    if(CMD_RESULT EQUAL 0)
        message(STATUS "run ${_script_path} success.")
    else()
 #       message(FATAL_ERROR "run ${_script_path} with error: ${CMD_ERROR}")
    endif()
endmacro()

# 编译zlib库
if(MSVC)
    set(ZLIB_BUILD_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR} /scripts/zlib_build.bat")
else()
    set(ZLIB_BUILD_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR} /scripts/zlib_build.sh")
endif()
run_3rdparty_build_script(${ZLIB_BUILD_SCRIPT} ${3RDPARTY_TARGET_DIR}/zlib)


