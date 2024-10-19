cmake_minimum_required(VERSION 3.20.0)
# 定义工程名称, 并指定语言为C++
project(HXCache LANGUAGES CXX)

# 是否开启 绕过操作系统的缓存, 直接将数据从用户空间读写到磁盘
# set(IO_URING_DIRECT ON) # 有BUG 先不要用

# 启用 搜集客户端地址信息 宏
add_definitions(-DCLIENT_ADDRESS_LOGGING)

# 是否系统已经安装了OpenSSL
set(HX_NET_FIND_OPENSSL ON)

# 是否启用 Address Sanitizer
set(HX_DEBUG_BY_ADDRESS_SANITIZER ON)

# 使用 Address Sanitizer
if(HX_DEBUG_BY_ADDRESS_SANITIZER AND CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
  message("=-=-=-=-=-=-= 启用 Address Sanitizer [-fsanitize=address] =-=-=-=-=-=-=")
endif()

#####        ########
# 请选择编译的示例代码#
########       ######
add_definitions(-DCACHE_TEST_MAIN) # 测试LRU缓存

# 如果没有定义CMAKE_BUILD_TYPE，则设置为Release
# 可以使用 cmake -DCMAKE_BUILD_TYPE=Debug .. 来指定
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    message("=-=-=-=-=-=-=当前是 [Release] 模式=-=-=-=-=-=-=")
else()
    message("=-=-=-=-=-=-=当前是 [DEBUG] 模式=-=-=-=-=-=-=")
    add_definitions(-D_HX_DEBUG_) # 添加一个 _HX_DEBUG_ 宏
    message("已添加宏: _HX_DEBUG_")
endif()

set(CMAKE_CXX_STANDARD 11) # 设置C++标准为C++20
set(CMAKE_C_STANDARD 11)   # 设置C语言标准为C11
set(CMAKE_CXX_STANDARD_REQUIRED ON) # 指定C++标准是必需的
set(CMAKE_CXX_EXTENSIONS OFF) # 禁用编译器的扩展
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines") # 显式启动C++协程

# 设置CMake模块路径, 包含当前目录下的cmake文件夹以及之前的路径
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake;${CMAKE_MODULE_PATH}")

add_compile_options(-Wall -Wextra -Werror=return-type)

# 定义生成的库的存放目录
set(LIB_PATH ${PROJECT_SOURCE_DIR}/lib)

set(ROOT_PATH "${PROJECT_SOURCE_DIR}") # 设置根项目的根目录
# 在camke .. 的时候会输出提示目录路径 (可选)
message (STATUS "Prefix dir is ${CMAKE_INSTALL_PREFIX}")
message (STATUS "Binary dir is ${PROJECT_BINARY_DIR}")
message (STATUS "Source dir is ${PROJECT_SOURCE_DIR}")
message (STATUS "ROOT_PATH dir is ${ROOT_PATH}")

# 设置文件名称
set (appName HXCache)

# 递归查找
file(GLOB_RECURSE srcs CONFIGURE_DEPENDS
    include/*.h 
    examples/*.cpp
)

# 编译可执行文件
add_executable (${appName} ${srcs})

# 添加WebSocket需要的Hash库, 以解析数据
target_include_directories(HXCache PUBLIC lib/hashlib/include)

# 设置头文件路径
include_directories(${ROOT_PATH}/include)

# 将include目录添加到HXHttp库的公共包含目录中
target_include_directories(HXCache PUBLIC include)

# 使用 Address Sanitizer
if(HX_DEBUG_BY_ADDRESS_SANITIZER AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(HXCache PRIVATE -fsanitize=address)
endif()