cmake_minimum_required(VERSION 3.0)
project(shaders)

# Find OpenGL headers and libraries
find_package(OpenGL REQUIRED)

# Set paths to GLEW library
set(GLEW_DIR "${CMAKE_CURRENT_LIST_DIR}/../lib/glew-1.13.0")
set(GLEW_INCLUDE_DIR "${GLEW_DIR}/include")
set(GLEW_LIB_DIR "${GLEW_DIR}/lib/Release/x64")
set(GLEW_LIB "${GLEW_LIB_DIR}/glew32.lib")

# Set paths for GLFW
set(GLFW_DIR "${CMAKE_CURRENT_LIST_DIR}/../lib/glfw-3.1.2.bin.WIN64")
set(GLFW_INCLUDE_DIR "${GLFW_DIR}/include")
if(MINGW)
    set(GLFW_LIB_DIR "${GLFW_DIR}/lib-mingw-w64")
elseif(MSVC11)
    set(GLFW_LIB_DIR "${GLFW_DIR}/lib-vc2012")
elseif(MSVC12)
    set(GLFW_LIB_DIR "${GLFW_DIR}/lib-vc2013")
elseif(MSVC14)
    set(GLFW_LIB_DIR "${GLFW_DIR}/lib-vc2015")
else()
    message(FATAL_ERROR "GLFW: platform not supported")
endif()
set(GLFW_LIB "${GLFW_LIB_DIR}/glfw3.lib")

# Set paths for GLM
set(GLM_DIR "${CMAKE_CURRENT_LIST_DIR}/../lib/glm")

# Build executable
add_executable(shaders main.cpp)
target_include_directories(shaders SYSTEM PRIVATE "${OPENGL_INCLUDE_DIR}" "${GLEW_INCLUDE_DIR}" "${GLFW_INCLUDE_DIR}" "${GLM_DIR}")
target_link_libraries(shaders "${OPENGL_gl_LIBRARY}" "${GLEW_LIB}" "${GLFW_LIB}")
