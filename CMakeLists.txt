cmake_minimum_required(VERSION 3.9)

project(ngin)
project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 14)

if(WIN32)
  message("Build for windows")
else()
  message("Build for unix")
endif(WIN32)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    message("Building project in release mode")
else()
    add_definitions(-DDEBUG=1)
    message("Building project in debug mode")
endif(NOT CMAKE_BUILD_TYPE)

if(WIN32)
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")

        file(COPY
                ${CMAKE_SOURCE_DIR}/third-party-libs/config.h
             DESTINATION
                ${CMAKE_SOURCE_DIR}/third-party-libs/assimp-master/include/assimp
             )

        add_compile_options(/bigobj)
        add_definitions(-D__WIN32__)

    else()
        message( FATAL_ERROR "Compiler not supported. You need to use visual studio compiler" )
    endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
else()
    set (CMAKE_CXX_FLAGS "-g -fno-inline -fno-omit-frame-pointer -m64")
endif(WIN32)

find_package(OpenGL REQUIRED)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/third-party-libs/glfw-master)
include_directories(${CMAKE_SOURCE_DIR}/third-party-libs/glfw-master/include)

if(WIN32)
include_directories(${CMAKE_SOURCE_DIR}/third-party-libs/glfw-master/deps)
endif(WIN32)

# assimp
add_subdirectory(${CMAKE_SOURCE_DIR}/third-party-libs/assimp-master)
include_directories(${CMAKE_SOURCE_DIR}/third-party-libs/assimp-master/include)

include_directories(${OPENGL_INCLUDE_DIR} ${GLFW3_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIRS})

set(PROJECT_SOURCE_FILES

        #hpp
        ${CMAKE_SOURCE_DIR}/libs/source/glMath.h
        ${CMAKE_SOURCE_DIR}/source_code/base/ngin.hpp
        ${CMAKE_SOURCE_DIR}/source_code/utils/loader.hpp
        ${CMAKE_SOURCE_DIR}/source_code/utils/utils.hpp
        ${CMAKE_SOURCE_DIR}/source_code/umbrellaHeader.hpp
        ${CMAKE_SOURCE_DIR}/source_code/utils/ogl/proxyOGL.hpp
        ${CMAKE_SOURCE_DIR}/source_code/utils/ogl/utilsOGL.hpp
        ${CMAKE_SOURCE_DIR}/source_code/base/ogl/nginOGL.hpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/shaders/ogl/shaderOGL.hpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/shaders/ogl/forwardShaderOGL.hpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/shaders/ogl/deferredShaderOGL.hpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/shaders/ogl/deferredLightingShaderOGL.hpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/shaders/shader.hpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/shaders/forwardShader.hpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/shaders/deferredShader.hpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/shaders/deferredLightingShader.hpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/shaders/lightSourceDrawerShader.hpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/shaders/ogl/lightSourceDrawerShaderOGL.hpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/ogl/forwardRenderOGL.hpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/forwardRender.hpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/ogl/deferredRenderOGL.hpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/ogl/framebufferOGL.hpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/framebuffer.hpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/ogl/renderbufferOGL.hpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/ogl/lightSourceRenderOGL.hpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/lightSourcesRender.hpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/render.hpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/renderbuffer.hpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/reusableGeometry.hpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/deferredRender.hpp
        ${CMAKE_SOURCE_DIR}/source_code/resources/resource.hpp
        ${CMAKE_SOURCE_DIR}/source_code/resources/resourceCoordinator.hpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/camera/perspectiveCamera.hpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/camera/camera.hpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/lights/pointLight.hpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/lights/spotLight.hpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/lights/directionalLight.hpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/lights/ogl/pointLightOGL.hpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/lights/ogl/spotLightOGL.hpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/lights/general/light.hpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/ogl/drawableOGL.hpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/ogl/geometryOGL.hpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/geometry.hpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/ogl/textureOGL.hpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/scene.hpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/material.hpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/drawable.hpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/positionedObject.hpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/texture.hpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/vertex.hpp

        #cpp
        ${CMAKE_SOURCE_DIR}/source_code/base/ngin.cpp
        ${CMAKE_SOURCE_DIR}/source_code/utils/loader.cpp
        ${CMAKE_SOURCE_DIR}/source_code/utils/ogl/proxyOGL.cpp
        ${CMAKE_SOURCE_DIR}/source_code/utils/ogl/utilsOGL.cpp
        ${CMAKE_SOURCE_DIR}/source_code/base/ogl/nginOGL.cpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/shaders/ogl/shaderOGL.cpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/shaders/ogl/forwardShaderOGL.cpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/shaders/ogl/deferredShaderOGL.cpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/shaders/ogl/deferredLightingShaderOGL.cpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/shaders/ogl/lightSourceDrawerShaderOGL.cpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/ogl/forwardRenderOGL.cpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/ogl/deferredRenderOGL.cpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/ogl/framebufferOGL.cpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/ogl/renderbufferOGL.cpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/ogl/lightSourceRenderOGL.cpp
        ${CMAKE_SOURCE_DIR}/source_code/rendering/deferredRender.cpp
        ${CMAKE_SOURCE_DIR}/source_code/resources/resource.cpp
        ${CMAKE_SOURCE_DIR}/source_code/resources/resourceCoordinator.cpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/camera/perspectiveCamera.cpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/ogl/drawableOGL.cpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/ogl/geometryOGL.cpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/geometry.cpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/ogl/textureOGL.cpp
        ${CMAKE_SOURCE_DIR}/source_code/scene/scene.cpp
        ${CMAKE_SOURCE_DIR}/example_projects/mac/main.cpp
        )

if (WIN32)
    set(PROJECT_SOURCE_FILES ${PROJECT_SOURCE_FILES}
            third-party-libs/renderdoc.h
            ${CMAKE_SOURCE_DIR}/third-party-libs/glfw-master/deps/KHR/khrplatform.h
            ${CMAKE_SOURCE_DIR}/third-party-libs/glfw-master/deps/glad/glad.h
            ${CMAKE_SOURCE_DIR}/third-party-libs/glfw-master/deps/glad.c
            )
endif (WIN32)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})

#renderdoc
file(COPY ${CMAKE_SOURCE_DIR}/third-party-libs/renderdoc.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#models
file(COPY ${CMAKE_SOURCE_DIR}/models/classroom DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/models/sponza DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/models/nanosuit DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/models/sphere DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/models/cone DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${PROJECT_NAME}  glfw     ${GLFW_LIBRARIES})
target_link_libraries(${PROJECT_NAME}  assimp   ${ASSIMP_LIBRARIES})

IF (APPLE)
    target_compile_definitions  (${PROJECT_NAME}  PRIVATE mac_build=1)
    target_link_libraries       (${PROJECT_NAME} "-framework CoreFoundation")
    target_link_libraries       (${PROJECT_NAME} "-framework CoreVideo")
    target_link_libraries       (${PROJECT_NAME} "-framework IOKit")
    target_link_libraries       (${PROJECT_NAME} "-framework Cocoa")
ENDIF(APPLE)

target_link_libraries       (${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_compile_definitions  (${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")
