cmake_minimum_required(VERSION 3.8)
project(application)

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
config_qt_compile(${PLATFORM_NAME})
message("Info: Project ${PROJECT_NAME} config qt compile finished")
if(DEBUG)
	add_definitions(-D_APP_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
else()
	add_definitions(-D_APP_SOURCE_DIR="./")
endif()

# 依赖的基础库头文件包含目录
# zutils
include_directories(../basic/zutils ../basic/zutils/include)
# zcore
include_directories(../basic/zcore  ../basic/zcore/include)
# zqtgui
include_directories(../basic/zqtui   ../basic/zqtui/include)
# zdatabase
include_directories(../basic/zdatabase   ../basic/zdatabase/include)
include_directories(../basic/pages  ../basic/pages/test_page)


# 拷贝依赖的基础库文件和页面动态库
list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../pages/bin/${TARGET_DIRNAME})
list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../service/bin/${TARGET_DIRNAME})
list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../logics/bin/${TARGET_DIRNAME})
list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../basic/bin/${TARGET_DIRNAME})

if(MSVC64)
    # 直接使用已经编译好的openssl二进制库
	list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../triLibrary/prelibrary/x64/openssl/lib)
	list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../triLibrary/prelibrary/x64/openssl/bin)
else()
	list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../triLibrary/prebuild/${TARGET_DIRNAME}/openssl/lib)
endif()

link_directories(${DEPEND_LIBRARY_PATHS})
copy_depend_libs("${DEPEND_LIBRARY_PATHS}")

# 需要编译的页面插件
add_subdirectory ("renderz")

