cmake_minimum_required(VERSION 3.4)
project(mqttMsgReceiver VERSION 0.1)
set (SOURCES_common main.c)
find_package(PkgConfig)

# add dependency libraries
set (LIBRARIES ${LIBRARIES}
    mosquitto
    pthread
    )

add_executable (msgReciever ${SOURCES_common})
target_link_libraries(msgReciever ${LIBRARIES})

target_include_directories(msgReciever PUBLIC)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
install (TARGETS msgReciever DESTINATION bin)
