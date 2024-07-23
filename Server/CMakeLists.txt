# CMakeList.txt: Server 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

# 将源代码添加到此项目的可执行文件。
add_executable (Server "main.cpp" "net/socket.h" "net/ipaddress.h" "net/ipaddress.cpp" "net/socket.cpp" "net/Channel.h" "net/Selector.h" "net/Select.h" "net/Select.cpp" "net/Channel.cpp" "net/EventLoop.h" "net/EventLoop.cpp" "net/Server.h" "net/Server.cpp" "net/Connection.h" "net/Connection.cpp" "net/Acceptor.h" "net/Acceptor.cpp"   "net/Epoll.h" "net/Epoll.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Server PROPERTY CXX_STANDARD 20)
endif()

include_directories(net/)

# TODO: 如有需要，请添加测试并安装目标。
