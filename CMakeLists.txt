cmake_minimum_required(VERSION 3.16)
project(boostGames)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_VERBOSE_MAKEFILE ON)
cmake_policy(SET CMP0074 NEW)

add_library(game_common INTERFACE)
if(MSVC)
target_compile_definitions(game_common INTERFACE -D_WIN32_WINNT=0x0601)
endif()


find_package(Threads)

# Boost.
# Note: For non standard location, define BOOST_ROOT
if(MSVC)
	set(Boost_DEBUG ON)
	set(Boost_COMPILER -vc142)
	set(Boost_USE_STATIC_LIBS ON)
	set(Boost_NO_SYSTEM_PATHS ON)
endif()
find_package(Boost REQUIRED COMPONENTS thread date_time log REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

message(STATUS "------------------------------")
message(STATUS "boost found: ${Boost_FOUND}")
message(STATUS "boost include dirs: ${Boost_INCLUDE_DIRS}")
message(STATUS "boost library dirs: ${Boost_LIBRARY_DIRS}")
message(STATUS "boost datetime: ${Boost_DATE_TIME_LIBRARY}")
message(STATUS "boost version: ${Boost_VERSION}")
message(STATUS)


add_executable(memory memory.cpp)
target_link_libraries(memory PRIVATE game_common)

add_executable(datetime datetime.cpp)
target_link_libraries(datetime PRIVATE game_common)

add_executable(wait wait.cpp)
target_link_libraries(wait PRIVATE game_common Boost::thread)

add_executable(th_wait th_wait.cpp)
target_link_libraries(th_wait PRIVATE game_common ${CMAKE_THREAD_LIBS_INIT} Boost::thread)

add_executable(asio_net asio_net.cpp)
target_link_libraries(asio_net PRIVATE game_common Boost::thread)

if(${Boost_VERSION} VERSION_LESS "1.70")
	message(STATUS "skipping program: 'request'. Beast requires version 70")
else()
	add_executable(request request.cpp)
    target_link_libraries(request PRIVATE game_common)
endif()

add_executable(timer timer.cpp)
target_link_libraries(timer PRIVATE game_common Boost::thread)
target_compile_definitions(timer PRIVATE -DBOOST_REGEX_NO_LIB)

add_executable(logging logging.cpp)
target_link_libraries(logging Boost::log Boost::filesystem)