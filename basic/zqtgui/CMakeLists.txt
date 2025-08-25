cmake_minimum_required (VERSION 3.8)
project(zqtgui)



include(qt_utils) 
config_qt_compile(${PLATFORM_NAME})
message("Info: Project ${PROJECT_NAME} config qt compile finished")

include_directories(../zutils ../zutils/include)
include_directories(../zcore ../zcore/include)
include_directories(../zmath ../zmath/include)
include_directories(../zrender ../zrender/include)
include_directories(../zimgui ../zimgui/include)

include_directories(include src)
include_directories(${triLibraryPath}/imgui)   # triLibraryPath 外部传入


if(MSVC)
	add_definitions(-D_ZQTGUI_EXPORTS)
endif()

file(GLOB_RECURSE H_HEADER_FILES "include/*.h")
set(HEADER_FILES ${HEADER_FILES} ${H_HEADER_FILES})
file(GLOB_RECURSE H_HEADER_FILES "src/*.h")
set(HEADER_FILES ${HEADER_FILES} ${H_HEADER_FILES})
file(GLOB_RECURSE HPP_HEADER_FILES "include/*.hpp")
set(HEADER_FILES ${HEADER_FILES} ${HPP_HEADER_FILES})

file(GLOB_RECURSE C_SOURCE_FILES "src/*.c")
set(SOURCE_FILES ${SOURCE_FILES} ${C_SOURCE_FILES})
file(GLOB_RECURSE CPP_SOURCE_FILES "src/*.cpp")
set(SOURCE_FILES ${SOURCE_FILES} ${CPP_SOURCE_FILES})

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::Core Qt5::Gui Qt5::OpenGL)
target_link_libraries(${PROJECT_NAME} PUBLIC zutils zmath zrender zcore zimgui)

target_compile_definitions(${PROJECT_NAME} PRIVATE ASSERT_PATH="${assertPath}")


if(WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE opengl32 glu32)
elseif(UNIX)
  target_link_libraries(${PROJECT_NAME} PRIVATE GL GLU)
  #glut
endif()

if(GNUC)
	target_link_libraries(${PROJECT_NAME} PRIVATE pthread rt dl)
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath='$ORIGIN:$ORIGIN/lib'")
endif()