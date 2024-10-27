cmake_minimum_required(VERSION 3.8)
project(triLibrary)

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


#三方库拷贝
if(MSVC64)
    list(APPEND TRILIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../triLibrary/jsoncpp/${TARGET_DIRNAME}/lib)
else()
    list(APPEND TRILIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../triLibrary/jsoncpp/${TARGET_DIRNAME}/lib)
endif()


foreach(LIB ${TRILIBRARY_PATHS})
    file(COPY "${LIB}" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}")
endforeach()