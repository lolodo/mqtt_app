cmake_minimum_required(VERSION 3.4)
project(mqttMsgReceiver VERSION 0.1)
set (SOURCES_common main.c)
find_package(PkgConfig)

# add dependency libraries
set (LIBRARIES ${LIBRARIES}
    mosquitto
    pthread
    json
    )

add_executable (msgReceiver ${SOURCES_common})
target_link_libraries(msgReceiver ${LIBRARIES})

target_include_directories(msgReceiver PUBLIC)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
install (TARGETS msgReceiver DESTINATION bin)

add_subdirectory(json)
