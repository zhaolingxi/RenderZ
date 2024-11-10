# 基础配置项
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 编译平台定义
if(PLATFORM_NAME STREQUAL "WIN32")
	set(MSVC True)
	set(MSVC32 True)
	add_definitions(-D_WIN32 -DWIN32)
elseif (PLATFORM_NAME STREQUAL "WIN64")
	set(MSVC True)
	set(MSVC64 True)
	add_definitions(-D_WIN64 -DWIN64 -D_WIN32 -DWIN32)
elseif (PLATFORM_NAME STREQUAL "LINUX32")
	set(GNUC True)
	set(GNUC32 True)
	add_definitions(-D_LINUX32 -DLINUX32 -DGNUC32 -D_GNUC32 -D_LINUX32 -DLINUX32 -D__linux32__ -D__LINUX32__)
elseif (PLATFORM_NAME STREQUAL "LINUX64")
	set(GNUC True)
	set(GNUC64 True)
	add_definitions(-D_LINUX64 -DLINUX64 -DGNUC64 -D_GNUC64 -D_LINUX64 -DLINUX64 -D__linux64__ -D__LINUX64__)
endif()

# 编译版本: Debug/Release
if (CMAKE_BUILD_TYPE STREQUAL "Debug") 
	set(DEBUG True)
elseif(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
	set(DEBUG True)
else()
	set(DEBUG False)
endif()

# 是否编译test
if (BUILD_TEST STREQUAL "True") 
	set(BUILD_TEST True)
else()
	set(BUILD_TEST False)
endif()

# 通用编译宏定义
if (MSVC)
	# Windows编译配置 _WIN32_WINNT=0x0601 --win7
	add_definitions(-D_WINDOWS -DWINDOWS -D_MSVC -DMSVC -D_WIN32_WINNT=0x0601 -DWIN32_LEAN_AND_MEAN)
elseif(GNUC)
	add_definitions(-D_LINUX -DLINUX  -DGNUC -D_GNUC -D__linux__ -D__unix__)
endif()
# 版本编译相关的宏定义
if(DEBUG)
	add_definitions(-D_DEBUG=1 -DDEBUG=1)
else()
	add_definitions(-D_NDEBUG=1 -DNDEBUG=1 -D_RELEASE=1 -DRELEASE=1)
endif()
