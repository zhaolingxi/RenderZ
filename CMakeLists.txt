#整体cmake设置
cmake_minimum_required (VERSION 3.14)
project ("RenderZ")


set(CMAKE_CXX_STANDARD 17)
set(MSVC_COMPILER "Visual Studio 17 2022" CACHE STRING "MSVC compiler")
set(GCC_COMPILER "Unix Makefiles" CACHE STRING "GCC compiler")

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


# cmake模块路径
set(CMAKE_MODULE_PATH
	${CMAKE_CURRENT_SOURCE_DIR}/makefile/cmake
	${CMAKE_CURRENT_SOURCE_DIR}/makefile/cmake/platforms/
)
# 编译默认配置和编译路径
include(build_utils)
include(global_config)
config_target_path(${CMAKE_CURRENT_SOURCE_DIR})
config_build_path(${CMAKE_CURRENT_SOURCE_DIR})

# 公共编译选项
include(common)
# 不同平台的编译选项
if (MSVC)
	message("Info: set windows ${PROJECT_NAME} compile flag, MSVC: ${MSVC}")
	include(windows)
    cmake_host_system_information(RESULT CPU_NUMBER_OF_LOGICAL_CORES QUERY NUMBER_OF_LOGICAL_CORES)
    add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP${CPU_NUMBER_OF_LOGICAL_CORES}>)
elseif(GNUC)
	message("Info: set linux ${PROJECT_NAME} compile flag, GNUC: ${GNUC}")
	include(linux)
endif()
message("---compile ${PROJECT_NAME} Platform: ${PLATFORM_NAME}, Debug: ${DEBUG}, CXX: ${CMAKE_CXX_COMPILER}")

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

if(MSVC)
    add_compile_options(/MP)
endif()

set(modelPath ${CMAKE_CURRENT_SOURCE_DIR}/model/)
set(assertPath ${CMAKE_CURRENT_SOURCE_DIR}/assert/)

# ==================== 新的代码：使用 FetchContent 管理 triLibrary ====================

# 1. 引入 FetchContent 模块
include(FetchContent)

# 2. 声明你的 triLibrary 仓库信息
FetchContent_Declare(
    triLibrary_dep # 给这个依赖起一个唯一的内部名字
    GIT_REPOSITORY "git@github.com:zhaolingxi/triLibrary.git" # 仓库地址
    #GIT_REPOSITORY "file:///home/zhaolx/zhaolx/renderz/new_renderz/tri_lib/triLibrary" # 可以进行本地替换
)

# 3. 让 CMake 下载并配置这个依赖
FetchContent_MakeAvailable(triLibrary_dep)

# 4. (为了兼容旧的子模块) 重新定义 triLibraryPath 变量
#    让它指向 FetchContent 下载下来的源代码目录
set(triLibraryPath ${trilibrary_dep_SOURCE_DIR}) # <--- 关键修改！使用 FetchContent 提供的变量
message("triLibraryPath ； "${triLibraryPath})
# =================================================================================

include_directories(${triLibraryPath}/plog/include/)


add_subdirectory ("basic") # 基础库
add_subdirectory ("pages") # 功能页面插件
add_subdirectory ("application")   # 应用程序

