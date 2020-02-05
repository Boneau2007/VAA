cmake_minimum_required(VERSION 3.15)
project(VAA)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)

include_directories(src)
include_directories(src/Messages/Include)
include_directories(src/Handler/Include)
include_directories(src/Application/Include)
include_directories(src/Graph/Include)


add_executable(networkNode
        src/Graph/Include/edge.hpp
        src/Graph/Include/node.hpp
        src/Graph/edge.cpp
        src/Graph/node.cpp
        src/IMessage.hpp
        src/messages.hpp
        src/Handler/Include/fileHandler.hpp
        src/Handler/Include/floodHandler.hpp
        src/Handler/floodHandler.cpp
        src/Handler/Include/messageHandler.hpp
        src/Handler/Include/echoHandler.hpp
        src/Handler/Include/accountHandler.hpp
        src/Handler/Include/goldmanEdgeChasingHandler.hpp
        src/Handler/Include/configHandler.hpp
        src/Handler/configHandler.cpp
        src/Handler/fileHandler.cpp
        src/Handler/echoHandler.cpp
        src/Handler/accountHandler.cpp
        src/Handler/goldmanEdgeChasingHandler.cpp
        src/Handler/messageHandler.cpp
        src/Application/Include/doubleCounting.hpp
        src/Application/Include/agreement.hpp
        src/Application/doubleCounting.cpp
        src/Application/agreement.cpp
        src/networkApp.hpp
        src/lamportClock.hpp
        src/networkApp.cpp)

add_executable(nodeApp
        src/Handler/Include/configHandler.hpp
        src/Graph/Include/edge.hpp
        src/IMessage.hpp
        src/messages.hpp
        src/Handler/Include/fileHandler.hpp
        src/Application/Include/agreement.hpp
        src/Handler/Include/messageHandler.hpp
        src/Handler/Include/echoHandler.hpp
        src/Handler/Include/accountHandler.hpp
        src/Handler/Include/goldmanEdgeChasingHandler.hpp
        src/Application/Include/doubleCounting.hpp
        src/Graph/Include/node.hpp
        src/Handler/configHandler.cpp
        src/Graph/edge.cpp
        src/Handler/Include/floodHandler.hpp
        src/Handler/floodHandler.cpp
        src/Handler/fileHandler.cpp
        src/Handler/messageHandler.cpp
        src/Handler/accountHandler.cpp
        src/Handler/echoHandler.cpp
        src/Handler/goldmanEdgeChasingHandler.cpp
        src/Application/doubleCounting.cpp
        src/Application/agreement.cpp
        src/Graph/node.cpp
        src/lamportClock.hpp
        src/nodeApp.cpp)

add_executable(graphGenApp
        src/Graph/Include/edge.hpp
        src/Graph/Include/node.hpp
        src/IMessage.hpp
        src/messages.hpp
        src/Handler/Include/fileHandler.hpp
        src/Handler/Include/messageHandler.hpp
        src/Handler/Include/echoHandler.hpp
        src/Handler/Include/accountHandler.hpp
        src/Handler/Include/goldmanEdgeChasingHandler.hpp
        src/Handler/Include/configHandler.hpp
        src/Application/Include/doubleCounting.hpp
        src/Application/Include/agreement.hpp
        src/Handler/Include/floodHandler.hpp
        src/Handler/floodHandler.cpp
        src/Handler/configHandler.cpp
        src/Graph/edge.cpp
        src/Handler/goldmanEdgeChasingHandler.cpp
        src/Application/agreement.cpp
        src/Graph/node.cpp
        src/Handler/fileHandler.cpp
        src/graphGenApp.cpp
        src/Handler/echoHandler.cpp
        src/Handler/accountHandler.cpp
        src/Application/doubleCounting.cpp
        src/lamportClock.hpp
        src/Handler/messageHandler.cpp)

install( TARGETS nodeApp graphGenApp)