cmake_minimum_required(VERSION 3.11)

project(findfaces
        VERSION 0.1
        DESCRIPTION "Example project using OpenCV to take input picture and circle faces found in picture")

add_definitions(-D_WIN32_WINNT=0x0501)

add_executable(findfaces faces.cpp server.cpp)

find_package(Threads)
find_package(fmt REQUIRED)
find_package(unofficial-http-parser REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(restinio REQUIRED)
find_package(OpenCV REQUIRED)

target_link_libraries(
    findfaces
    PRIVATE
        ${CMAKE_THREAD_LIBS_INIT}
        fmt::fmt
        unofficial::http_parser::http_parser
        ${Boost_LIBRARIES}
        restinio::restinio
        ${OpenCV_LIBS}
)

set_property(TARGET findfaces PROPERTY CXX_STANDARD 14)

configure_file(haarcascade_frontalface_alt2.xml haarcascade_frontalface_alt2.xml COPYONLY)
