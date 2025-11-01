cmake_minimum_required (VERSION 3.8)
project(zdatabase)

include_directories(../zutils ../zutils/include)
include_directories(../zcore ../zcore/include)
include_directories(include src)
include_directories(${triLibraryPath})

if(MSVC)
	add_definitions(-D_ZDATABASE_EXPORTS)
endif()

file(GLOB HEADER_FILES ${HEADER_FILES} "include/*.h")  # 头文件
file(GLOB SOURCE_FILES ${SOURCE_FILES} "src/*.cpp")    # 源文件


add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PUBLIC zutils zcore)


file(GLOB SQLITE_LIBS "${triLibraryPath}/prebuildlib/${TARGET_DIRNAME}/sqlite3/lib/*sqlite3*")
message(STATUS "Found sqlite3 libs: ${SQLITE_LIBS}")
if(SQLITE_LIBS)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${SQLITE_LIBS})
else()
#    message(FATAL_ERROR "sqlite3 library not found")
endif()

if(GNUC)
	target_link_libraries(${PROJECT_NAME} PRIVATE pthread rt dl)
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath='$ORIGIN:$ORIGIN/lib'")
endif()

