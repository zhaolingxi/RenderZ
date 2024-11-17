cmake_minimum_required(VERSION 3.5)
project(pages)



# Qt编译选项
include(qt_utils)
find_package(Qt5 REQUIRED COMPONENTS OpenGL PrintSupport)
config_qt_compile(${PLATFORM_NAME})
message("Info: Project ${PROJECT_NAME} config qt compile finished")
if(DEBUG)
	add_definitions(-D_APP_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
else()
	add_definitions(-D_APP_SOURCE_DIR="./")
endif()

# 依赖的基础库头文件包含目录
include_directories(../basic/zutils ../basic/zutils/include)
include_directories(../basic/zcore ../basic/zcore/include)
include_directories(../basic/zmath ../basic/zmath/include)
include_directories(../basic/zrender ../basic/zrender/include)
include_directories(../basic/zqtgui ../basic/zqtgui/include)


# 拷贝依赖的基础库文件和业务模块
list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../logics/bin/${TARGET_DIRNAME})
list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../basic/bin/${TARGET_DIRNAME})


link_directories(${DEPEND_LIBRARY_PATHS})
copy_depend_libs("${DEPEND_LIBRARY_PATHS}") # 拷贝依赖的基础库文件


if(DEBUG)
    add_definitions(-D_PAGES_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
else()
    add_definitions(-D_PAGES_SOURCE_DIR="./")
endif()

# 需要编译的页面插件
add_subdirectory ("renderz_main_page")
if(BUILD_TEST)
#	add_subdirectory ("test_page")
endif()

