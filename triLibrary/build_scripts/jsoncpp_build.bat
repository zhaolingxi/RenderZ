@echo off
rem cmake工具
set CMAKE_TOOL=%1%
rem Debug或Release
set BUILD_CONFIGURE=%2%
rem 生成文件保存目录
set INSTALL_DIR=%3%
rem *******************************************************************************
cd ..
echo enter the 3rdparty home dir: %cd%
echo cmake tools: %CMAKE_TOOL%
echo build config: %BUILD_CONFIGURE%
set BUILD_DIR_NAME=build
cd /D jsoncpp
echo build jsoncpp dir: %cd%
if exist "%BUILD_DIR_NAME%" (
 	rd /s /Q "%BUILD_DIR_NAME%"
)
md "%BUILD_DIR_NAME%"
cd "%BUILD_DIR_NAME%"
rem 编译选项
set JSONCPP_BUILD_CONFIGURE=Debug
if "%BUILD_CONFIGURE%"=="Release" (
	set JSONCPP_BUILD_CONFIGURE=RelWithDebInfo
)
rem 因为3方库通常都不会一直更改,所以只要已经编译完成,则直接退出
if exist "%INSTALL_DIR%\include\json\json.h" (
    echo jsoncpp has compiled success quit directly.......
    exit /B
)

rem jsoncpp编译结果保存目录
if not exist %INSTALL_DIR% (
	md "%INSTALL_DIR%"
)

rem 编译jsoncpp库
%CMAKE_TOOL% -G "Visual Studio 17 2022" -A x64 -DCMAKE_INSTALL_PREFIX=%INSTALL_DIR% ..
%CMAKE_TOOL% --build . --target INSTALL --config %JSONCPP_BUILD_CONFIGURE%
echo jsoncpp build finished, install dir: %INSTALL_DIR%
