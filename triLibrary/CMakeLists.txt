cmake_minimum_required(VERSION 3.8)
project(triLibrary)

# 编译结果保存目录
set(3RDPARTY_TARGET_DIR ${CMAKE_CURRENT_SOURCE_DIR}/prebuildlib/${TARGET_DIRNAME})
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
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build_scripts)
    else()
        execute_process(COMMAND sh ${_script_path} "${CMAKE_COMMAND}" "${3RDPARTY_BUILD_CONFIG}" "${_install_dir}" 
                        RESULT_VARIABLE CMD_RESULT
                        OUTPUT_VARIABLE CMD_OUTPUT
                        ERROR_VARIABLE CMD_ERROR
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build_scripts)
    endif()
    message(STATUS "${_script_path} output: ${CMD_OUTPUT}")
    if(CMD_RESULT EQUAL 0)
        message(STATUS "run ${_script_path} success.")
    else()
        message(FATAL_ERROR "run ${_script_path} with error: ${CMD_ERROR}")
    endif()
endmacro()

# 编译sqlite3库
message("start compile sqlite3")
message("3RDPARTY_TARGET_DIR ${3RDPARTY_TARGET_DIR}")
if(MSVC)
    set(SQLITE_BUILD_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/build_scripts/sqlite3_build.bat")
else()
    set(SQLITE_BUILD_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/build_scripts/sqlite3_build.sh")
endif()
run_3rdparty_build_script(${SQLITE_BUILD_SCRIPT} ${3RDPARTY_TARGET_DIR}/sqlite3)

# 编译zlib库
message("start compile zilb1.3")
if(MSVC)
    set(ZLIB_BUILD_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/build_scripts/zlib_build.bat")
else()
    set(ZLIB_BUILD_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/build_scripts/zlib_build.sh")
endif()
run_3rdparty_build_script(${ZLIB_BUILD_SCRIPT} ${3RDPARTY_TARGET_DIR}/zlib)

# 编译jsoncpp库
message("start compile jsoncpp")
if(MSVC)
    set(JSONCPP_BUILD_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/build_scripts/jsoncpp_build.bat")
else()
    set(JSONCPP_BUILD_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/build_scripts/jsoncpp_build.sh")
endif()
run_3rdparty_build_script(${JSONCPP_BUILD_SCRIPT} ${3RDPARTY_TARGET_DIR}/jsoncpp)

# 编译jsoncpp库
message("start compile tinyxml2")
if(MSVC)
    set(TINYXML_BUILD_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/build_scripts/tinyxml_build.bat")
else()
    set(TINYXML_BUILD_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/build_scripts/tinyxml_build.sh")
endif()
run_3rdparty_build_script(${TINYXML_BUILD_SCRIPT} ${3RDPARTY_TARGET_DIR}/tinyxml2)

# 编译gsl库
message("start compile gsl2.7")
if(MSVC)
    set(GSL_BUILD_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/build_scripts/gsl_build.bat")
else()
    set(GSL_BUILD_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/build_scripts/gsl_build.sh")
endif()
run_3rdparty_build_script(${GSL_BUILD_SCRIPT} ${3RDPARTY_TARGET_DIR}/gsl2.7)

#三方库拷贝
foreach(LIB ${TRILIBRARY_PATHS})
    file(COPY "${LIB}" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}")
endforeach()


add_custom_target(triLibraryDone ALL
    DEPENDS ${TRILIBRARY_PATHS} # 包含了triLibrary中的所有目标
    COMMENT "All triLibrary targets built"
)