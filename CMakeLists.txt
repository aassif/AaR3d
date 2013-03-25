# Required CMake version.

cmake_minimum_required(VERSION 2.8)

# Project name.

project(AaR3d C CXX)

include("$ENV{DEVEL_PATH}/cmake/cmake-paths.txt")
include("$ENV{DEVEL_PATH}/cmake/cmake-env.txt"  )

if(MSVC)
    set(CMAKE_CXX_FLAGS "/EHa")
endif(MSVC)

# Dependencies.

find_package(OpenGL     REQUIRED)
find_package(GLEW       REQUIRED)
find_package(AaToolkit  REQUIRED)
find_package(AaOpenGL   REQUIRED)
find_package(AaOpenGLs  REQUIRED)
find_package(AaMesh     REQUIRED)

# Include directoires.

include_directories(
    include
    ${AAMESH_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${AATOOLKIT_INCLUDE_DIR}
    ${AAOPENGL_INCLUDE_DIR}
)

# Files.

FILE(GLOB HEADER_FILES include/*.h  )
FILE(GLOB SOURCE_FILES src/*.cc     )

# Shared target generation.

add_library(AaR3d
    SHARED
    ${HEADER_FILES}
    ${SOURCE_FILES}
    ${GLEW_FILES}
)

set_target_properties(AaR3d
    PROPERTIES
    COMPILE_DEFINITIONS AA_R3D_BUILD_SHARED
)

target_link_libraries(AaR3d
    ${OPENGL_LIBRARY}
    ${AAOPENGL_LIBRARY}
)

# Static target generation.
add_library(AaR3ds
    STATIC
    ${HEADER_FILES}
    ${SOURCE_FILES}
    ${GLEW_FILES}
)

target_link_libraries(AaR3ds
    ${OPENGL_LIBRARY}
    ${AAOPENGL_STATIC_LIBRARY}
)
