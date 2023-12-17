@echo off
rem ===============================================================================
rem * zlib编译脚本                                                         *
rem * 1. 编译需要将如下工具的目录修改为本机对应的目录                             *
rem * 2. 需要将目录的 '\' 修改为: '/'  或 '\\'                                    *
rem ===============================================================================
rem cmake工具
set CMAKE_TOOL="C:/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe"
rem Debug或Release
set BUILD_CONFIGURE=Release
rem 生成文件保存目录
set INSTALL_DIR=%cd%/../../prebuild/x64-debug/zlib
rem *******************************************************************************
cd ..
echo enter the triLibrary home dir: %cd%
echo cmake tools: %CMAKE_TOOL%
echo build config: %BUILD_CONFIGURE%
set BUILD_DIR_NAME=build
cd /D zlib-1.3
echo build zlib dir: %cd%
if exist "%BUILD_DIR_NAME%" (
 	rd /s /Q "%BUILD_DIR_NAME%"
)
md "%BUILD_DIR_NAME%"
cd "%BUILD_DIR_NAME%"
rem 编译选项
set ZLIB_BUILD_CONFIGURE=Debug
if "%BUILD_CONFIGURE%"=="Release" (
	set ZLIB_BUILD_CONFIGURE=RelWithDebInfo
)
rem 因为3方库通常都不会一直更改,所以只要已经编译完成,则直接退出
if exist "%INSTALL_DIR%\include\zlib.h" (
    echo zlib has compiled success quit directly.......
    exit /B
)

rem zlib编译结果保存目录
if not exist %INSTALL_DIR% (
	md "%INSTALL_DIR%"
)

rem 编译zlib库
%CMAKE_TOOL% -G "Visual Studio 17 2022" -A x64 -DCMAKE_INSTALL_PREFIX=%INSTALL_DIR% ..
%CMAKE_TOOL% --build . --target INSTALL --config %ZLIB_BUILD_CONFIGURE%
echo zlib build finished, install dir: %INSTALL_DIR%
