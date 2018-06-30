cmake_minimum_required(VERSION 3.9)

project(ngin)
project(${PROJECT_NAME} CXX)
project(${PROJECT_NAME} VERSION 0.0.1 DESCRIPTION "NGin, 3D graphics library")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#docs
option(BUILD_DOC "Build documentation" ON)

find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")
    add_custom_target( doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM )
    file(COPY ${CMAKE_SOURCE_DIR}/docs/DoxygenLayout.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY ${CMAKE_SOURCE_DIR}/docs/customdoxygen.css DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY ${CMAKE_SOURCE_DIR}/docs/brief.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    add_definitions(-DNDEBUG)
    message("Building project in release mode")
else()
    add_definitions(-DDEBUG=1)
    message("Building project in debug mode")
endif(NOT CMAKE_BUILD_TYPE)

if(WIN32)
    message("Build for windows")
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        add_compile_options(/bigobj)
        add_definitions(-D__WIN32__)
    else()
        message(FATAL_ERROR "Compiler not supported. You need to use visual studio compiler")
    endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
else()
    message("Build for unix")
    set (CMAKE_CXX_FLAGS "-g -fno-inline -fno-omit-frame-pointer -m64 -Wc++1z-extensions")
endif(WIN32)

find_package(OpenGL REQUIRED)

add_subdirectory(${CMAKE_SOURCE_DIR}/third-party-libs/assimp)
add_subdirectory(${CMAKE_SOURCE_DIR}/third-party-libs/glMath)
add_subdirectory(${CMAKE_SOURCE_DIR}/third-party-libs/aTree)

include_directories(${CMAKE_SOURCE_DIR}/third-party-libs/assimp/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/third-party-libs/assimp/include)
include_directories(${ASSIMP_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/third-party-libs/gLMath)
include_directories(${CMAKE_SOURCE_DIR}/third-party-libs/GSL/include)
include_directories(${CMAKE_SOURCE_DIR}/third-party-libs)
include_directories(${CMAKE_SOURCE_DIR}/third-party-libs/aTree)

set(NGIN_LIBRARY_SOURCE_FILES   ${CMAKE_SOURCE_DIR}/ngin/rendering/postprocessing/postprocessRender.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/base/ngin.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/rendering/postprocessing/gaussianBlurPostprocessRender.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/rendering/postprocessing/vignettePostprocessRender.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/rendering/render.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/utils/loader.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/utils/proxyOGL.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/rendering/shaders/shader.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/rendering/other/framebuffer.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/rendering/other/renderbuffer.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/rendering/deferredRender.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/scene/lights/light.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/scene/lights/spotLight.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/scene/lights/directionalLight.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/scene/lights/pointLight.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/rendering/shadowRender.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/rendering/modelRender.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/resources/resource.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/rendering/forwardRender.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/rendering/lightSourcesRender.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/resources/resourceCoordinator.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/scene/camera/perspectiveCamera.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/scene/camera/orthographicCamera.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/scene/material.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/scene/drawable.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/scene/geometry.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/scene/texture.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/scene/cubemap.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/scene/cubemapArray.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/scene/textureArray.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/scene/textureUtilities.cpp
                                ${CMAKE_SOURCE_DIR}/ngin/scene/scene.cpp)

if (WIN32)

    set(NGIN_LIBRARY_HEADER_FILES   ${NGIN_LIBRARY_HEADER_FILES}
                                    ${CMAKE_SOURCE_DIR}/third-party-libs/glfw/deps/KHR/khrplatform.h
                                    ${CMAKE_SOURCE_DIR}/third-party-libs/glfw/deps/glad/glad.h)
    set(NGIN_LIBRARY_SOURCE_FILES   ${NGIN_LIBRARY_SOURCE_FILES}
            ${CMAKE_SOURCE_DIR}/third-party-libs/glfw/deps/glad.c)

endif (WIN32)

add_library(${PROJECT_NAME} STATIC ${NGIN_LIBRARY_SOURCE_FILES})
set_target_properties(ngin PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION 1)

add_dependencies(${PROJECT_NAME} glMath)
add_dependencies(${PROJECT_NAME} aTree)

target_link_libraries(${PROJECT_NAME} assimp ${ASSIMP_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

IF (APPLE)
    target_compile_definitions(${PROJECT_NAME}  PRIVATE mac_build=1)
    target_link_libraries(${PROJECT_NAME} "-framework CoreFoundation")
    target_link_libraries(${PROJECT_NAME} "-framework CoreVideo")
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
ENDIF(APPLE)

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/ngin
        DESTINATION include
        FILES_MATCHING PATTERN "*.hpp *.h")


############################### DEMO DEMO DEMO DEMO ###############################

### GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/third-party-libs/glfw)
include_directories(${CMAKE_SOURCE_DIR}/third-party-libs/glfw/include)

if(WIN32)
include_directories(${CMAKE_SOURCE_DIR}/third-party-libs/glfw/deps)
endif(WIN32)

include_directories(${OPENGL_INCLUDE_DIR} ${GLFW3_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR})


set(TEST_PROJECT_SOURCE_FILES   ${CMAKE_SOURCE_DIR}/example_projects/main.cpp
                                ${CMAKE_SOURCE_DIR}/example_projects/controlls.cpp
                                ${CMAKE_SOURCE_DIR}/example_projects/utils.cpp
                                ${CMAKE_SOURCE_DIR}/example_projects/controlls.hpp
                                ${CMAKE_SOURCE_DIR}/example_projects/utils.hpp)

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
file(COPY ${CMAKE_SOURCE_DIR}/models/sponza DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/models/nanosuit DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/models/sphere DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/models/cone DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if (WIN32)
#    file(COPY ${CMAKE_CURRENT_BINARY_DIR}/third-party-libs/glfw/src/glfw3.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#    file(COPY ${CMAKE_CURRENT_BINARY_DIR}/third-party-libs/assimp/code/glfw3.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

target_link_libraries(testapp glfw ${GLFW_LIBRARIES})
target_link_libraries(testapp ${PROJECT_NAME})

target_compile_definitions  (${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")

############################### DEMO DEMO DEMO DEMO ###############################