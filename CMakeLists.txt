cmake_minimum_required(VERSION 3.9)

project(ngin)
project(${PROJECT_NAME} CXX)
project(${PROJECT_NAME} VERSION 0.0.1 DESCRIPTION "NGin, 3D graphics library")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32)
  message("Build for windows")
else()
  message("Build for unix")
endif(WIN32)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    add_definitions(-DNDEBUG)
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
                ${CMAKE_SOURCE_DIR}/third-party-libs/assimp/include/assimp
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

# assimp
add_subdirectory(${CMAKE_SOURCE_DIR}/third-party-libs/assimp)

include_directories(${CMAKE_SOURCE_DIR}/third-party-libs/assimp/include)
include_directories(${OPENGL_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/third-party-libs/GSL/include)

set(NGIN_LIBRARY_HEADER_FILES   ${CMAKE_SOURCE_DIR}/glMath/source/glMath.h
                                ${CMAKE_SOURCE_DIR}/source_code/base/ngin.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/utils/loader.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/utils/utils.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/umbrellaHeader.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/config.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/utils/proxyOGL.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/utils/utils.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/rendering/shaders/shader.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/rendering/forwardRender.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/rendering/other/framebuffer.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/rendering/lightSourcesRender.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/rendering/render.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/rendering/other/renderbuffer.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/rendering/reusableGeometry.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/rendering/deferredRender.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/resources/resource.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/resources/resourceCoordinator.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/scene/camera/perspectiveCamera.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/scene/camera/orthographicCamera.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/scene/camera/camera.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/scene/lights/pointLight.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/scene/lights/spotLight.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/scene/lights/directionalLight.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/scene/lights/light.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/scene/geometry.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/scene/scene.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/scene/material.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/scene/drawable.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/scene/texture.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/scene/vertex.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/rendering/postprocessDrawer.hpp
                                ${CMAKE_SOURCE_DIR}/source_code/rendering/shadowRender.hpp)

set(NGIN_LIBRARY_SOURCE_FILES   ${CMAKE_SOURCE_DIR}/source_code/rendering/postprocessDrawer.cpp
                                ${CMAKE_SOURCE_DIR}/source_code/base/ngin.cpp
                                ${CMAKE_SOURCE_DIR}/source_code/rendering/render.cpp
                                ${CMAKE_SOURCE_DIR}/source_code/utils/loader.cpp
                                ${CMAKE_SOURCE_DIR}/source_code/utils/proxyOGL.cpp
                                ${CMAKE_SOURCE_DIR}/source_code/rendering/shaders/shader.cpp
                                ${CMAKE_SOURCE_DIR}/source_code/rendering/other/framebuffer.cpp
                                ${CMAKE_SOURCE_DIR}/source_code/rendering/other/renderbuffer.cpp
                                ${CMAKE_SOURCE_DIR}/source_code/rendering/deferredRender.cpp
                                ${CMAKE_SOURCE_DIR}/source_code/rendering/shadowRender.cpp
                                ${CMAKE_SOURCE_DIR}/source_code/resources/resource.cpp
                                ${CMAKE_SOURCE_DIR}/source_code/rendering/forwardRender.cpp
                                ${CMAKE_SOURCE_DIR}/source_code/rendering/lightSourcesRender.cpp
                                ${CMAKE_SOURCE_DIR}/source_code/resources/resourceCoordinator.cpp
                                ${CMAKE_SOURCE_DIR}/source_code/scene/camera/perspectiveCamera.cpp
                                ${CMAKE_SOURCE_DIR}/source_code/scene/camera/orthographicCamera.cpp
                                ${CMAKE_SOURCE_DIR}/source_code/scene/material.cpp
                                ${CMAKE_SOURCE_DIR}/source_code/scene/drawable.cpp
                                ${CMAKE_SOURCE_DIR}/source_code/scene/geometry.cpp
                                ${CMAKE_SOURCE_DIR}/source_code/scene/texture.cpp
                                ${CMAKE_SOURCE_DIR}/source_code/scene/scene.cpp)

add_library(${PROJECT_NAME} ${NGIN_LIBRARY_SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION 1 PUBLIC_HEADER ${NGIN_LIBRARY_HEADER_FILES})

target_link_libraries(${PROJECT_NAME} assimp ${ASSIMP_LIBRARIES})

IF (APPLE)
    target_compile_definitions(${PROJECT_NAME}  PRIVATE mac_build=1)
    target_link_libraries(${PROJECT_NAME} "-framework CoreFoundation")
    target_link_libraries(${PROJECT_NAME} "-framework CoreVideo")
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
ENDIF(APPLE)

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

# TEST_APP

## GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/third-party-libs/glfw)
include_directories(${CMAKE_SOURCE_DIR}/third-party-libs/glfw/include)

if(WIN32)
include_directories(${CMAKE_SOURCE_DIR}/third-party-libs/glfw/deps)
endif(WIN32)

include_directories(${OPENGL_INCLUDE_DIR} ${GLFW3_INCLUDE_DIRS})

set(TEST_PROJECT_SOURCE_FILES   ${CMAKE_SOURCE_DIR}/example_projects/mac/main.cpp
                                ${CMAKE_SOURCE_DIR}/example_projects/mac/controlls.cpp
                                ${CMAKE_SOURCE_DIR}/example_projects/mac/utils.cpp
                                ${CMAKE_SOURCE_DIR}/example_projects/mac/controlls.hpp
                                ${CMAKE_SOURCE_DIR}/example_projects/mac/utils.hpp)

if (WIN32)
    set(TEST_PROJECT_SOURCE_FILES   ${TEST_PROJECT_SOURCE_FILES}
                                    third-party-libs/renderdoc.h
                                    ${CMAKE_SOURCE_DIR}/third-party-libs/glfw/deps/KHR/khrplatform.h
                                    ${CMAKE_SOURCE_DIR}/third-party-libs/glfw/deps/glad/glad.h
                                    ${CMAKE_SOURCE_DIR}/third-party-libs/glfw/deps/glad.c)
endif (WIN32)

add_executable(testapp ${TEST_PROJECT_SOURCE_FILES})
add_dependencies(testapp ${PROJECT_NAME})

#renderdoc
file(COPY ${CMAKE_SOURCE_DIR}/third-party-libs/renderdoc.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#models
file(COPY ${CMAKE_SOURCE_DIR}/models/classroom DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/models/sponza DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/models/nanosuit DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/models/sphere DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/models/cone DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(testapp glfw ${GLFW_LIBRARIES})
target_link_libraries(testapp ${PROJECT_NAME})

target_compile_definitions  (${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")
