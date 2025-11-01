cmake_minimum_required (VERSION 3.8)
project(zrender)

# -----------------------------------------
# �ռ�����Ŀ�Լ���ͷ�ļ� / Դ�ļ� (�ⲿ�ֲ���)
# -----------------------------------------
file(GLOB_RECURSE H_IN_INCLUDE   "include/*.h")
file(GLOB_RECURSE H_IN_SRC       "src/*.h")
file(GLOB_RECURSE HPP_IN_INCLUDE "include/*.hpp")
set(HEADER_FILES
    ${H_IN_INCLUDE}
    ${H_IN_SRC}
    ${HPP_IN_INCLUDE}
)

file(GLOB_RECURSE C_SOURCES   "src/*.c")
file(GLOB_RECURSE CPP_SOURCES "src/*.cpp")
set(SOURCE_FILES
    ${C_SOURCES}
    ${CPP_SOURCES}
)

# -----------------------------------------
# ������
# -----------------------------------------
add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})

# -----------------------------------------
# �������޸ġ�Ϊ��Ŀ�����Ӱ���Ŀ¼
# -----------------------------------------

# Eigen �ǹ�����������Ϊ����ͷ�ļ������ǵĹ���ͷ�ļ��б�������
target_include_directories(${PROJECT_NAME} PUBLIC
    ${triLibraryPath}/eigen-3.4.0
)

# ��Щ�� zrender �ڲ�ʵ����Ҫ�ģ���������������������
# ���� zutils, zcore, zmath ��ͷ�ļ�Ҳ���ܱ� zrender �Ĺ���ͷ�ļ�������Ҳ��Ϊ PUBLIC
target_include_directories(${PROJECT_NAME} PUBLIC
    ../zutils/include
    ../zcore/include
    ../zmath/include
    ../zimgui/include # ��� zimgui �ǹ��������Ļ�
)

# ��Щ�� zrender �Լ���Ŀ¼��Ҳ��Ҫ��¶��ʹ����
target_include_directories(${PROJECT_NAME} PUBLIC
    include
)

# ��Щ�� zrender �Լ���˽��ʵ��Ŀ¼����Ϊ PRIVATE ����
target_include_directories(${PROJECT_NAME} PRIVATE
    src
)


if(MSVC)
	# ��Ӧ���� target_compile_definitions
	target_compile_definitions(${PROJECT_NAME} PRIVATE _ZRENDER_EXPORTS)
endif()

target_link_libraries(${PROJECT_NAME}  PUBLIC zutils zmath zcore)
if(GNUC)
	target_link_libraries(${PROJECT_NAME} PRIVATE pthread rt dl)
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath='$ORIGIN:$ORIGIN/lib'")
endif()