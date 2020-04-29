SET(pwd ${CMAKE_CURRENT_LIST_DIR})

SET(src "" texture_to_render.cpp radiosityScene.cpp radiosityScene.h)
AUX_SOURCE_DIRECTORY(${pwd} src)
add_executable(skinning ${src})
message(STATUS "skinning added ${src}")

target_link_libraries(skinning ${stdgl_libraries})
FIND_PACKAGE(JPEG REQUIRED)
TARGET_LINK_LIBRARIES(skinning ${JPEG_LIBRARIES})
TARGET_LINK_LIBRARIES(skinning pmdreader)
