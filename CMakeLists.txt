cmake_minimum_required(VERSION 3.8)
project(sketchCanvas)

if (WIN32)
    set(GLFW_INCLUDE_DIRS "C:/Program Files (x86)/glfw-3.3.8/include")
    set(GLFW_LIBRARY_DIRS "C:/Program Files (x86)/glfw-3.3.8/lib")
endif (WIN32)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package (OpenGL REQUIRED)
if (APPLE)
    find_package(glfw3 3.3 REQUIRED)
endif (APPLE)

if (WIN32)
    include_directories(${OPENGL_INCLUDE_DIR}  ${GLFW_INCLUDE_DIRS})
    link_directories(${GLFW_LIBRARY_DIRS})
elseif (APPLE)
    include_directories(${OPENGL_INCLUDE_DIR}  ${GLFW_INCLUDE_DIRS})
endif ()

file(GLOB SOURCE_FILES
        *.cpp
        *.h
        )

add_executable(sketchCanvas ${SOURCE_FILES})

if (WIN32)
    target_link_libraries (sketchCanvas ${OPENGL_LIBRARIES} glfw3dll)
elseif (APPLE)
    target_link_libraries(sketchCanvas ${OPENGL_LIBRARIES} glfw)
endif()