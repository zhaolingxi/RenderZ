
cmake_minimum_required (VERSION 3.8)
project ("basic")

link_directories(${DEPEND_LIBRARY_PATHS})
copy_depend_libs("${DEPEND_LIBRARY_PATHS}") # 拷贝依赖的基础库文件

# 设置basic源码所在的目录
add_definitions(-D_BASIC_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# 添加子项目
add_subdirectory ("zutils")
add_subdirectory ("zcore")
add_subdirectory ("zqtgui")
add_subdirectory ("zdatabase")
