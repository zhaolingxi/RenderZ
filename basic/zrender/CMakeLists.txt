cmake_minimum_required (VERSION 3.8)
project(zrender)

# -----------------------------------------
# 收集本项目自己的头文件 / 源文件 (这部分不变)
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
# 创建库
# -----------------------------------------
add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})

# -----------------------------------------
# 【核心修改】为库目标添加包含目录
# -----------------------------------------

# Eigen 是公开依赖，因为它的头文件在我们的公开头文件中被包含了
target_include_directories(${PROJECT_NAME} PUBLIC
    ${triLibraryPath}/eigen-3.4.0
)

# 这些是 zrender 内部实现需要的，或者是其他公开的依赖
# 假设 zutils, zcore, zmath 的头文件也可能被 zrender 的公开头文件包含，也设为 PUBLIC
target_include_directories(${PROJECT_NAME} PUBLIC
    ../zutils/include
    ../zcore/include
    ../zmath/include
    ../zimgui/include # 如果 zimgui 是公开依赖的话
)

# 这些是 zrender 自己的目录，也需要暴露给使用者
target_include_directories(${PROJECT_NAME} PUBLIC
    include
)

# 这些是 zrender 自己的私有实现目录，设为 PRIVATE 即可
target_include_directories(${PROJECT_NAME} PRIVATE
    src
)


if(MSVC)
	# 这应该用 target_compile_definitions
	target_compile_definitions(${PROJECT_NAME} PRIVATE _ZRENDER_EXPORTS)
endif()

target_link_libraries(${PROJECT_NAME}  PUBLIC zutils zmath zcore)
if(GNUC)
	target_link_libraries(${PROJECT_NAME} pthread rt dl)
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath='$ORIGIN:$ORIGIN/lib'")
endif()