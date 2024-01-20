cmake_minimum_required(VERSION 3.5)
project(pages)

# cmake模块路径
set(CMAKE_MODULE_PATH
        ${CMAKE_CURRENT_SOURCE_DIR}/../makefile/cmake
        ${CMAKE_CURRENT_SOURCE_DIR}/../makefile/cmake/platforms/
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

# Qt编译选项
include(qt_utils)
find_package(Qt5 REQUIRED COMPONENTS OpenGL PrintSupport)
config_qt_compile(${PLATFORM_NAME})
message("Info: Project ${PROJECT_NAME} config qt compile finished")

# 依赖的基础库头文件包含目录
# aautils
include_directories(../basic/aautils ../basic/zutils/include)
# aacore
include_directories(../basic/aacore ../basic/zcore/include)
# triLibrary 
if(MSVC64)
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../triLibrary/prelibrary/x64/danetsdk/include)
else()
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../triLibrary/prelibrary/linux64/danetsdk/include)
endif()

# 拷贝依赖的基础库文件和业务模块
list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../logics/bin/${TARGET_DIRNAME})
list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../basic/bin/${TARGET_DIRNAME})
# zmq
list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/prebuild/${TARGET_DIRNAME}/zmq/bin)
list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/prebuild/${TARGET_DIRNAME}/zmq/lib)
# openssl
if(MSVC64)
    # 直接使用已经编译好的openssl二进制库
    list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/prelibrary/x64/openssl/lib)
    list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/prelibrary/x64/openssl/bin)
else()
    list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/prebuild/${TARGET_DIRNAME}/openssl/lib)
endif()

# danetsdk
if(MSVC64)
	if(DEBUG)
		list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/prelibrary/x64/danetsdk/debug/lib)
		list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/prelibrary/x64/danetsdk/debug/bin)
	else()
		list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/prelibrary/x64/danetsdk/release/lib)
		list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/prelibrary/x64/danetsdk/release/bin)
	endif()
else()
	if(DEBUG)
		list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/prelibrary/linux64/danetsdk/debug/lib)
	else()
		list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/prelibrary/linux64/danetsdk/release/lib)
	endif()
endif()

link_directories(${DEPEND_LIBRARY_PATHS})
copy_depend_libs("${DEPEND_LIBRARY_PATHS}") # 拷贝依赖的基础库文件


# 画布依赖的组件资源文件
if(DEBUG)
    add_definitions(-D_PAGES_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
else()
    add_definitions(-D_PAGES_SOURCE_DIR="./")
endif()

# 需要编译的页面插件
add_subdirectory ("test_page")
if(BUILD_TEST)
#	add_subdirectory ("apppage_test")
endif()

