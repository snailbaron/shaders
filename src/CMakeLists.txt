cmake_minimum_required(VERSION 3.0)
project(shaders)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../bin")

# Find OpenGL headers and libraries
find_package(OpenGL REQUIRED)

# Set paths to GLEW library
set(GLEW_DIR "${CMAKE_CURRENT_LIST_DIR}/../lib/glew-1.13.0")
set(GLEW_INCLUDE_DIR "${GLEW_DIR}/include")
set(GLEW_LIB_DIR "${GLEW_DIR}/lib/Release/x64")
set(GLEW_LIB "${GLEW_LIB_DIR}/glew32.lib")
set(GLEW_DLL "${GLEW_DIR}/bin/Release/x64/glew32.dll")

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

# Set paths for Boost
set(BOOST_DIR "${CMAKE_CURRENT_LIST_DIR}/../lib/boost_1_60_0")
set(BOOST_LIB_DIR "${BOOST_DIR}/stage/lib")
set(BOOST_DEBUG_LIBS
    "${BOOST_LIB_DIR}/libboost_filesystem-vc140-mt-gd-1_60.lib"
    "${BOOST_LIB_DIR}/libboost_system-vc140-mt-gd-1_60.lib")
set(BOOST_RELEASE_LIBS
    "${BOOST_LIB_DIR}/libboost_filesystem-vc140-mt-1_60.lib"
    "${BOOST_LIB_DIR}/libboost_system-vc140-mt-1_60.lib")

# Build executable
set(SOURCES
    main.cpp
    error.hpp
    errors.hpp
    glsl_exception.hpp
    glsl_program.hpp
    glsl_program.cpp
    basic.vert
    basic.frag
    diffuse.vert
    diffuse.frag
)
add_executable(shaders ${SOURCES})
target_include_directories(shaders SYSTEM PRIVATE "${OPENGL_INCLUDE_DIR}" "${GLEW_INCLUDE_DIR}" "${GLFW_INCLUDE_DIR}" "${GLM_DIR}" "${BOOST_DIR}")
target_link_libraries(shaders "${OPENGL_gl_LIBRARY}" "${GLEW_LIB}" "${GLFW_LIB}" debug ${BOOST_DEBUG_LIBS} optimized ${BOOST_RELEASE_LIBS})

# Copy required DLLs after executable build
get_property(BIN_DIR TARGET shaders PROPERTY RUNTIME_OUTPUT_DIRECTORY)
add_custom_command(TARGET shaders POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy "${GLEW_DLL}" "${BIN_DIR}/Debug")
add_custom_command(TARGET shaders POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy "${GLEW_DLL}" "${BIN_DIR}/Release")