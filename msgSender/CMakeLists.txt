cmake_minimum_required(VERSION 3.4)
project(mqttMsgSender VERSION 0.1)
set (SOURCES_common main.c)
find_package(PkgConfig)

# add dependency libraries
set (LIBRARIES ${LIBRARIES}
    mosquitto
    pthread
    )

add_executable (msgSender ${SOURCES_common})
target_link_libraries(msgSender ${LIBRARIES})

target_include_directories(msgSender PUBLIC)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
install (TARGETS msgSender DESTINATION bin)
