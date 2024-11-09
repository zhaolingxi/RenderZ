cmake_minimum_required (VERSION 3.8)
project(zqtgui)

# Qt编译选项
include(qt_utils) 
config_qt_compile(${PLATFORM_NAME})
message("Info: Project ${PROJECT_NAME} config qt compile finished")

# 依赖库头文件包含目录
include_directories(../zutils/include ../zcore/include)
# project的头文件包含目录
include_directories(include src)
include_directories(include/zqtviewer src/zqtviewer)
include_directories(include/zqtnavigator src/zqtnavigator)
include_directories(include/zqt3dcoordinatesystem src/zqt3dcoordinatesystem)

if(MSVC)
	add_definitions(-D_ZQTGUI_EXPORTS)
endif()
#add_compile_definitions(${PROJECT_NAME} PRIVATE _ZQTGUI_EXPORTS)

# 项目头文件
file(GLOB_RECURSE H_HEADER_FILES "include/*.h")
set(HEADER_FILES ${HEADER_FILES} ${H_HEADER_FILES})
file(GLOB_RECURSE H_HEADER_FILES "src/*.h")
set(HEADER_FILES ${HEADER_FILES} ${H_HEADER_FILES})
file(GLOB_RECURSE HPP_HEADER_FILES "include/*.hpp")
set(HEADER_FILES ${HEADER_FILES} ${HPP_HEADER_FILES})

# 项目源文件
file(GLOB_RECURSE C_SOURCE_FILES "src/*.c")
set(SOURCE_FILES ${SOURCE_FILES} ${C_SOURCE_FILES})
file(GLOB_RECURSE CPP_SOURCE_FILES "src/*.cpp")
set(SOURCE_FILES ${SOURCE_FILES} ${CPP_SOURCE_FILES})

# 特别提醒: 需要添加 ${HEADER_FILES},因为moc生成的文件会保存到 ${HEADER_FILES}
add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
# 框架基础库
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::Core Qt5::Gui Qt5::OpenGL)
target_link_libraries(${PROJECT_NAME} PRIVATE zutils zcore)

if(WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE opengl32 glu32)
elseif(UNIX)
  target_link_libraries(${PROJECT_NAME} PRIVATE GL GLU glut)
endif()

if(GNUC)
	target_link_libraries(${PROJECT_NAME} PRIVATE pthread rt dl)
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath='$ORIGIN:$ORIGIN/lib'")
endif()