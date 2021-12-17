cmake_minimum_required(VERSION 3.0.0)
project(alphagameengine VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)

set(HEADERS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/include/*)
include(CTest)


find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(GLEW 2.0 REQUIRED)
find_package(spdlog CONFIG REQUIRED)

include_directories(${HEADERS_DIRECTORY})
enable_testing()
find_package(OpenGL REQUIRED)
if (APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OPENGL_LIBRARY OpenGL)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    set(EXTRA_LIBS
            ${COCOA_LIBRARY}
            ${OPENGL_LIBRARY}
            ${IOKIT_LIBRARY}
            ${COREVIDEO_LIBRARY}
            )
endif (APPLE)


add_executable(alphagameengine src/main.cpp src/include/entity.h src/include/entities/cube_entity.h src/include/stb_image.h src/include/shaderClass.cpp src/include/shaderClass.h src/include/glad/glad.h src/include/KHR/khrplatform.h src/glad.c)
target_link_libraries(alphagameengine PRIVATE ${EXTRA_LIBS}  glm::glm glfw GLEW::GLEW spdlog::spdlog spdlog::spdlog_header_only)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/assets/
        ${CMAKE_CURRENT_BINARY_DIR}/assets/)