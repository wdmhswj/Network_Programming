# 最低版本限制
cmake_minimum_required(VERSION 3.15.0)
# 项目名称
project(hello)
# 输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 生成可执行程序
add_executable(server hello_server.c)
add_executable(client hello_client.c)
