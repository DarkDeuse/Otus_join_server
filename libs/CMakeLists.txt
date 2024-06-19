cmake_minimum_required(VERSION 3.2)

add_library(joinServerLib STATIC joinServer.h joinServer.cpp)
set_target_properties(joinServerLib  PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
target_link_libraries(joinServerLib PRIVATE)
target_include_directories(joinServerLib PRIVATE ${CMAKE_BINARY_DIR})

add_library(clientServer STATIC Client.cpp Client.h Server.cpp Server.h)
set_target_properties(clientServer  PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
target_link_libraries(clientServer PRIVATE)
target_include_directories(clientServer PRIVATE ${CMAKE_BINARY_DIR})

if(MSVC)
	target_compile_options(clientServer PRIVATE /W4)
else()
	target_compile_options(clientServer PRIVATE -Wall -Wextra -Werror)
endif()
