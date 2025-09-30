cmake_minimum_required(VERSION 3.5)
project(renderz_test)

# Qt编译选项
include(qt_utils) 
config_qt_compile(${PLATFORM_NAME})
message("Info: Project ${PROJECT_NAME} config qt compile finished")

file(GLOB_RECURSE HEADER_FILES "*.h")     # 头文件
file(GLOB_RECURSE SOURCE_FILES "*.cpp")   # 源文件
# 资源文件
file(GLOB_RECURSE RCC_SOURCE_FILES "resource/*.qrc")

add_library(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${RCC_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}  Qt5::Widgets  Qt5::Core Qt5::Gui)
target_link_libraries(${PROJECT_NAME} zutils zcore zqtgui zdatabase)
if(GNUC)
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath='$ORIGIN:$ORIGIN/lib'") 
endif()
