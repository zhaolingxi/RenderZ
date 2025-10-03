cmake_minimum_required(VERSION 3.5)
project(renderz)



include(qt_utils) 
config_qt_compile(${PLATFORM_NAME})
message("Info: Project ${PROJECT_NAME} config qt compile finished")

include_directories(../renderz_test/)
file(GLOB_RECURSE HEADER_FILES "*.h")     # 头文件
file(GLOB_RECURSE SOURCE_FILES "*.cpp")   # 源文件

file(GLOB_RECURSE RESOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/../../assert/*"
)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${RESOURCE_FILES})

set(RESOURCES_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../assert")

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${RESOURCES_SOURCE_DIR}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assert"
    COMMENT "Copying page resources to DLL output directory..."
)

target_link_libraries(${PROJECT_NAME}  Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Svg)
target_link_libraries(${PROJECT_NAME} zutils zcore zqtgui zdatabase renderz_test renderz_main_page)
if(GNUC)
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath='$ORIGIN:$ORIGIN/lib'") 
endif()
