@echo off
rem cmake����
set CMAKE_TOOL=%1%
rem Debug��Release
set BUILD_CONFIGURE=%2%
rem �����ļ�����Ŀ¼
set INSTALL_DIR=%3%
rem *******************************************************************************
cd ..
echo enter the 3rdparty home dir: %cd%
echo cmake tools: %CMAKE_TOOL%
echo build config: %BUILD_CONFIGURE%
set BUILD_DIR_NAME=build
cd /D sqlite3
echo build sqlite3 dir: %cd%
if exist "%BUILD_DIR_NAME%" (
 	rd /s /Q "%BUILD_DIR_NAME%"
)
md "%BUILD_DIR_NAME%"
cd "%BUILD_DIR_NAME%"
rem ����ѡ��
set SQLITE3_BUILD_CONFIGURE=Debug
if "%BUILD_CONFIGURE%"=="Release" (
	set SQLITE3_BUILD_CONFIGURE=RelWithDebInfo
)
rem ��Ϊ3����ͨ��������һֱ����,����ֻҪ�Ѿ��������,��ֱ���˳�
if exist "%INSTALL_DIR%\include\sqlite3.h" (
    echo sqlite3 has compiled success quit directly.......
    exit /B
)

rem sqlite3����������Ŀ¼
if not exist %INSTALL_DIR% (
	md "%INSTALL_DIR%"
)

rem ����sqlite3��
%CMAKE_TOOL% -G "Visual Studio 17 2022" -A x64 -DCMAKE_INSTALL_PREFIX=%INSTALL_DIR% ..
%CMAKE_TOOL% --build . --target INSTALL --config %SQLITE3_BUILD_CONFIGURE%
echo sqlite3 build finished, install dir: %INSTALL_DIR%