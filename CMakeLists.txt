cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 17)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
else()
    add_definitions(-DDEBUG=1)
    set (CMAKE_CXX_FLAGS "-g -fno-inline -fno-omit-frame-pointer -m64")
endif(NOT CMAKE_BUILD_TYPE)

project(ngin)

find_package(OpenGL REQUIRED)

# GLFW
add_subdirectory(${CMAKE_SOURCE_DIR}/third-party-libs/glfw-master)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include_directories(${CMAKE_SOURCE_DIR}/third-party-libs/glfw-master/include)
include_directories(${CMAKE_SOURCE_DIR}/third-party-libs/glfw-master/deps)

include_directories(${OPENGL_INCLUDE_DIR} ${GLFW3_INCLUDE_DIRS})


add_executable(${PROJECT_NAME}

        ${CMAKE_SOURCE_DIR}/libs/source/glMath.h
        ${CMAKE_SOURCE_DIR}/source_code/base/ngin.cpp
        ${CMAKE_SOURCE_DIR}/source_code/base/ogl/nginOGL.cpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/shaders/ogl/shaderOGL.cpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/shaders/ogl/ForwardShaderOGL.cpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/ogl/forwardRenderOGL.cpp
        ${CMAKE_SOURCE_DIR}/source_code/resources/resource.cpp
        ${CMAKE_SOURCE_DIR}/source_code/resources/resourceCoordinator.cpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/camera/perspectiveCamera.cpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/ogl/sceneNodeOGL.cpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/ogl/geometryOGL.cpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/scene.cpp

        ${CMAKE_SOURCE_DIR}/example_projects/mac/main.cpp
        ${CMAKE_SOURCE_DIR}/third-party-libs/glfw-master/deps/KHR/khrplatform.h
        ${CMAKE_SOURCE_DIR}/third-party-libs/glfw-master/deps/glad/glad.h
        ${CMAKE_SOURCE_DIR}/third-party-libs/glfw-master/deps/glad.c
        third-party-libs/renderdoc.h
        )

#renderdoc
file(COPY ${CMAKE_SOURCE_DIR}/third-party-libs/renderdoc.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/source_code/shaders/forward_render/vshader.glsl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/source_code/shaders/forward_render/fshader.glsl DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


target_link_libraries(${PROJECT_NAME}  glfw ${GLFW_LIBRARIES})

IF (APPLE)
    target_compile_definitions  (${PROJECT_NAME}  PRIVATE mac_build=1)
    target_link_libraries       (${PROJECT_NAME} "-framework CoreFoundation")
    target_link_libraries       (${PROJECT_NAME} "-framework CoreVideo")
    target_link_libraries       (${PROJECT_NAME} "-framework IOKit")
    target_link_libraries       (${PROJECT_NAME} "-framework Cocoa")
ENDIF()

target_link_libraries       (${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_compile_definitions  (${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")