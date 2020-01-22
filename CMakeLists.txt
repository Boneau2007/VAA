cmake_minimum_required(VERSION 3.15)
project(VAA)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)
include_directories(src/include)

add_executable(networkNode
        src/include/edge.hpp
        src/include/fileHandler.hpp
        src/include/message.hpp
        src/include/messageHandler.hpp
        src/include/networkApp.hpp
        src/include/node.hpp
        src/edge.cpp
        src/fileHandler.cpp
        src/message.cpp
        src/messageHandler.cpp
        src/networkApp.cpp
        src/node.cpp)

add_executable(nodeApp
        src/include/edge.hpp
        src/include/fileHandler.hpp
        src/include/message.hpp
        src/include/messageHandler.hpp
        src/include/node.hpp
        src/edge.cpp
        src/fileHandler.cpp
        src/message.cpp
        src/messageHandler.cpp
        src/node.cpp
        src/nodeApp.cpp)

add_executable(graphGenApp
        src/include/edge.hpp
        src/include/fileHandler.hpp
        src/include/message.hpp
        src/include/messageHandler.hpp
        src/include/node.hpp
        src/edge.cpp
        src/fileHandler.cpp
        src/graphGenApp.cpp
        src/message.cpp
        src/messageHandler.cpp
        src/node.cpp)
