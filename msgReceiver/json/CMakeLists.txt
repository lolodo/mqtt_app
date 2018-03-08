set (CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
add_library(json STATIC
    json.hpp
    json_parser.cpp
    )
