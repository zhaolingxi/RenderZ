cmake_minimum_required(VERSION 3.8)
project(zimgui)

# -----------------------------------------
# 头文件搜索路径
# -----------------------------------------
include_directories(../zutils ../zutils/include)
include_directories(../zcore ../zcore/include)
include_directories(../zmath ../zmath/include)
include_directories(../zrender ../zrender/include)

include_directories(include src)
include_directories(${triLibraryPath}/imgui)   # triLibraryPath 外部传入

# -----------------------------------------
# 宏定义
# -----------------------------------------
if(MSVC)
#    add_definitions(-D_ZIMGUI_EXPORTS)
endif()

# -----------------------------------------
# 收集本项目自己的头文件 / 源文件
# -----------------------------------------
file(GLOB_RECURSE H_IN_INCLUDE   "include/*.h")
file(GLOB_RECURSE H_IN_SRC       "src/*.h")
file(GLOB_RECURSE HPP_IN_INCLUDE "include/*.hpp")
set(HEADER_FILES
    ${H_IN_INCLUDE}
    ${H_IN_SRC}
    ${HPP_IN_INCLUDE}
)

file(GLOB_RECURSE C_SOURCES   "src/*.c")
file(GLOB_RECURSE CPP_SOURCES "src/*.cpp")
set(SOURCE_FILES
    ${C_SOURCES}
    ${CPP_SOURCES}
)

# -----------------------------------------
# ImGui 公共源文件
# -----------------------------------------
set(IMGUI_DIR ${triLibraryPath}/imgui)
set(IMGUI_COMMON
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
)

# -----------------------------------------
# 根据平台选择正确的 ImGui 后端
# -----------------------------------------
if(ANDROID)
    list(APPEND IMGUI_BACKEND
        ${IMGUI_DIR}/backends/imgui_impl_android.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    )
elseif(WIN32 OR UNIX)       # Windows / Linux / macOS
    # 假设你用 GLFW；如果实际用 SDL2 把 glfw 行换成 sdl2 即可
    list(APPEND IMGUI_BACKEND
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    )
endif()

# -----------------------------------------
# 创建动态库
# -----------------------------------------
add_library(${PROJECT_NAME} STATIC
    ${HEADER_FILES}
    ${SOURCE_FILES}
    ${IMGUI_COMMON}
    ${IMGUI_BACKEND}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    zutils zmath zrender zcore
)

# -----------------------------------------
# OpenGL 系统库
# -----------------------------------------
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE opengl32 glu32)
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE GL GLU)
endif()

# -----------------------------------------
# Linux 额外链接选项
# -----------------------------------------
if(CMAKE_COMPILER_IS_GNUCXX)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread rt dl)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS "-Wl,-rpath='$ORIGIN:$ORIGIN/lib'")
endif()

include(FetchContent)

FetchContent_Declare(
    glfw # 给这个依赖起个名字，就叫 glfw
    SOURCE_DIR ${triLibraryPath}/glfw # 告诉它源码在本地的哪个路径
)

FetchContent_MakeAvailable(glfw) # 这一步会自动调用 add_subdirectory 并创建 glfw target


target_include_directories(${PROJECT_NAME} PRIVATE ${triLibraryPath}/glfw/include)


# -----------------------------------------
# 链接本项目依赖的库
# -----------------------------------------
target_link_libraries(${PROJECT_NAME} PRIVATE
    zutils zmath zrender zcore
    glfw
)