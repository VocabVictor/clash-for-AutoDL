cmake_minimum_required(VERSION 3.10)
project(clash_manager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# 查找依赖包
find_package(CURL REQUIRED)
find_package(yaml-cpp REQUIRED)

# 包含头文件目录
include_directories(
    ${CURL_INCLUDE_DIRS}
    ${YAML_CPP_INCLUDE_DIR}
)

# 添加可执行文件
add_executable(clash_manager src/clash_manager.cpp)

# 链接库
target_link_libraries(clash_manager
    PRIVATE
    ${CURL_LIBRARIES}
    yaml-cpp
    stdc++fs
)

# 安装
install(TARGETS clash_manager
    RUNTIME DESTINATION bin
) 