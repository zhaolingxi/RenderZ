cmake_minimum_required(VERSION 3.8)
project(application)


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
include_directories(../basic/zutils ../basic/zutils/include)
include_directories(../basic/zmath ../basic/zmath/include)
include_directories(../basic/zcore  ../basic/zcore/include)
include_directories(../basic/zrender ../basic/zrender/include)
include_directories(../basic/zqtgui   ../basic/zqtgui/include)
include_directories(../basic/zdatabase   ../basic/zdatabase/include)

include_directories(../pages/renderz_main_page/framework  ../pages/renderz_main_page/implement)


# 拷贝依赖的基础库文件和页面动态库
list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../pages/bin/${TARGET_DIRNAME})
list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../service/bin/${TARGET_DIRNAME})
list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../logics/bin/${TARGET_DIRNAME})
list(APPEND DEPEND_LIBRARY_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../basic/bin/${TARGET_DIRNAME})



link_directories(${DEPEND_LIBRARY_PATHS})
copy_depend_libs("${DEPEND_LIBRARY_PATHS}")


# 设置包含预构建库的根目录
set(TRILIBRARY_PATH ../triLibrary/prebuildlib)

# 使用 GLOB_RECURSE 递归查找所有子目录中的动态库和静态库文件
# 您可以根据需要调整文件扩展名
file(GLOB_RECURSE LIBRARIES
    "${TRILIBRARY_PATH}/*.so"
    "${TRILIBRARY_PATH}/*.a"
    "${TRILIBRARY_PATH}/*.dll"
    "${TRILIBRARY_PATH}/*.lib"
)

# 遍历文件列表并将每个文件拷贝到目标路径
foreach(LIB ${LIBRARIES})
    file(COPY "${LIB}" DESTINATION "${TARGET_PATH}")
endforeach()



# 需要编译的页面插件
add_subdirectory ("renderz")

