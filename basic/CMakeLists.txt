
cmake_minimum_required (VERSION 3.8)
project ("basic")

# 设置basic源码所在的目录
#add_definitions(-D_BASIC_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

# 添加子项目
add_subdirectory ("zutils")
add_subdirectory("zmath")
add_subdirectory ("zcore")
add_subdirectory ("zrender")
add_subdirectory ("zqtgui")
add_subdirectory ("zimgui")
add_subdirectory ("zdatabase")
