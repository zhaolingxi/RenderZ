message("Info: Compile ${PROJECT_NAME} for Linux.... platform: ${PLATFORM_NAME}")
# Linux编译配置项flags
set(CMAKE_C_FLAGS     "${CMAKE_C_FLAGS} -no-pie -Wall -w -fPIC -pthread -fsigned-char -fpermissive")
set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -no-pie -Wall -w -fPIC -pthread -fsigned-char -fpermissive")

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

