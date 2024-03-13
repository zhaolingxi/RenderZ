#####################################################################################
# description: 应用程序基础框架
#####################################################################################
cmake_minimum_required (VERSION 3.8)
project ("basic")
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

if(MSVC64)
	include_directories(../triLibrary/prelibrary/x64/openssl/include)
else()
	include_directories(../triLibrary/prebuild/${TARGET_DIRNAME}/openssl/include)
endif()

if(MSVC64)
    # 直接使用已经编译好的openssl二进制库
	list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../triLibrary/prelibrary/x64/openssl/lib)
	list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../triLibrary/prelibrary/x64/openssl/bin)
else()
	list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../triLibrary/prebuild/${TARGET_DIRNAME}/openssl/lib)
endif()
#list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/bin/${TARGET_DIRNAME})

link_directories(${DEPEND_LIBRARY_PATHS})
copy_depend_libs("${DEPEND_LIBRARY_PATHS}") # 拷贝依赖的基础库文件

# 设置basic源码所在的目录
add_definitions(-D_BASIC_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# 添加子项目
add_subdirectory ("zutils")
add_subdirectory ("zcore")
add_subdirectory ("zqtgui")
add_subdirectory ("zdatabase")
#add_subdirectory ("zrender")
