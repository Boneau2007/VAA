cmake_minimum_required(VERSION 3.15)
project(VAA)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)
include_directories(src/include)
add_executable(networkNode
        src/include/edge.hpp
        src/include/message.hpp
        src/include/fileHandler.hpp
        src/include/messageHandler.hpp
        src/include/echoHandler.hpp
        src/include/doubleCounting.hpp
        src/include/networkApp.hpp
        src/include/node.hpp
        src/include/agreement.hpp
        src/include/config.hpp
        src/config.cpp
        src/edge.cpp
        src/fileHandler.cpp
        src/message.cpp
        src/echoHandler.cpp
        src/doubleCounting.cpp
        src/agreement.cpp
        src/messageHandler.cpp
        src/networkApp.cpp
        src/node.cpp)

add_executable(nodeApp
        src/include/config.hpp
        src/include/edge.hpp
        src/include/message.hpp
        src/include/fileHandler.hpp
        src/include/agreement.hpp
        src/include/messageHandler.hpp
        src/include/echoHandler.hpp
        src/include/doubleCounting.hpp
        src/include/node.hpp
        src/config.cpp
        src/edge.cpp
        src/message.cpp
        src/fileHandler.cpp
        src/messageHandler.cpp
        src/echoHandler.cpp
        src/doubleCounting.cpp
        src/agreement.cpp
        src/node.cpp
        src/nodeApp.cpp)

add_executable(graphGenApp
        src/include/edge.hpp
        src/include/message.hpp
        src/include/fileHandler.hpp
        src/include/messageHandler.hpp
        src/include/echoHandler.hpp
        src/include/doubleCounting.hpp
        src/include/agreement.hpp
        src/include/node.hpp
        src/include/config.hpp
        src/config.cpp
        src/edge.cpp
        src/message.cpp
        src/agreement.cpp
        src/node.cpp
        src/fileHandler.cpp
        src/graphGenApp.cpp
        src/echoHandler.cpp
        src/doubleCounting.cpp
        src/messageHandler.cpp)

install( TARGETS nodeApp graphGenApp)