# cmake_minimum_required(VERSION 3.5)
# project(renderz_main_page)

# # =============== 新增: Qt 集成三件套 ===============
# # 明确查找 Qt5 包
# find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui)

# # 开启 AUTOMOC, AUTOUIC, 和 AUTORCC
# # AUTORCC 是这里的关键，它会自动调用 rcc 处理 .qrc 文件
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTORCC ON)
# # ================================================

# # project的头文件包含目录
# include_directories(framework interface implement)
# if(MSVC)
# 	add_compile_definitions(${PROJECT_NAME} PRIVATE _RENDERZ_MAIN_PAGE_EXPORTS)
# endif()
# # 项目头文件
# file(GLOB_RECURSE H_HEADER_FILES "framework/*.h")
# set(HEADER_FILES ${HEADER_FILES} ${H_HEADER_FILES})
# file(GLOB_RECURSE H_HEADER_FILES "interface/*.h")
# set(HEADER_FILES ${HEADER_FILES} ${H_HEADER_FILES})
# file(GLOB_RECURSE H_HEADER_FILES "implement/*.h")
# set(HEADER_FILES ${HEADER_FILES} ${H_HEADER_FILES})

# # 项目源文件
# file(GLOB_RECURSE CPP_SOURCE_FILES "framework/*.cpp")
# set(SOURCE_FILES ${SOURCE_FILES} ${CPP_SOURCE_FILES})
# file(GLOB_RECURSE CPP_SOURCE_FILES "implement/*.cpp")
# set(SOURCE_FILES ${SOURCE_FILES} ${CPP_SOURCE_FILES})

# # 生成ts翻译文件
# set(PRO_TS_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.ts")
# set(PRO_QM_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../resources/language/zh/${PROJECT_NAME}_zh.qm")
# update_qtpro_ts("${PRO_TS_FILE_PATH}" "${SOURCE_FILES}")
# update_qtpro_qm("${PRO_TS_FILE_PATH}" "${PRO_QM_FILE_PATH}")


# add_library(${PROJECT_NAME} SHARED 
#     ${HEADER_FILES} 
#     ${SOURCE_FILES}
#     renderz_main_page.qrc  # <-- 新增：将qrc文件本身作为源文件添加
# )


# # 框架基础库
# target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::Core Qt5::Gui)
# target_link_libraries(${PROJECT_NAME} PRIVATE zutils zmath zcore zrender zqtgui)

# target_compile_definitions(${PROJECT_NAME} PRIVATE MODEL_PATH="${modelPath}")
# target_compile_definitions(${PROJECT_NAME} PRIVATE ASSERT_PATH="${assertPath}")

# if(GNUC)
# 	target_link_libraries(${PROJECT_NAME} PRIVATE pthread rt dl)
# 	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath='$ORIGIN:$ORIGIN/lib'")
# endif()


cmake_minimum_required(VERSION 3.5)
project(renderz_main_page)

# ---------- Qt ----------
find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui LinguistTools)  # 新增 LinguistTools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ---------- 文件搜集 ----------
include_directories(framework interface implement)
file(GLOB_RECURSE H_FILES framework/*.h interface/*.h implement/*.h)
file(GLOB_RECURSE CPP_FILES framework/*.cpp implement/*.cpp)

# ---------- 翻译 ----------
set(TS_FILE  "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.ts")
set(QM_DIR   "${CMAKE_CURRENT_SOURCE_DIR}/../resources/language/zh")   # 产出目录
set(QM_FILE  "${QM_DIR}/${PROJECT_NAME}_zh.qm")

# 让 lrelease 先跑，生成 .qm
qt5_add_translation(QM_FILES ${TS_FILE})



# ---------- 翻译 ----------
set(TS_FILE  "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.ts")
set(QM_DIR   "${CMAKE_CURRENT_SOURCE_DIR}/../resources/language/zh")
set(QM_FILE  "${QM_DIR}/${PROJECT_NAME}_zh.qm")

# 1. 生成 .qm 的命令（输出在源码树，rcc 能找到）
add_custom_command(
    OUTPUT  ${QM_FILE}
    COMMAND ${Qt5_LRELEASE_EXECUTABLE} -qm ${QM_FILE} ${TS_FILE}
    DEPENDS ${TS_FILE}
    COMMENT "Generating ${PROJECT_NAME}_zh.qm"
)

# 2. 让 rcc 的生成规则依赖这个 .qm
#    办法：把 .qm 当作“源文件”挂到目标上
add_library(${PROJECT_NAME} SHARED
    ${H_FILES}
    ${CPP_FILES}
    ${QM_FILE}               # <-- 关键：让构建系统先执行上面那条命令
    renderz_main_page.qrc
)

# ---------- 链接 ----------
target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt5::Widgets Qt5::Core Qt5::Gui
    PRIVATE zutils zmath zcore zrender zqtgui
)

target_compile_definitions(${PROJECT_NAME} PRIVATE MODEL_PATH="${modelPath}")
target_compile_definitions(${PROJECT_NAME} PRIVATE ASSERT_PATH="${assertPath}")

if(GNUC)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread rt dl)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS "-Wl,-rpath,'$ORIGIN:$ORIGIN/lib'")
endif()