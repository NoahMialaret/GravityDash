cmake_minimum_required(VERSION 3.5.0)
project(GravDash VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin/)
include_directories(include include/glm)
file(GLOB SOURCES "src/*.cpp" "src/glad.c")

#add_library(GravDashObject.o OBJECT ${SOURCES})

add_executable(GravDash ${SOURCES})

target_link_libraries(GravDash PRIVATE glfw GL X11 pthread Xrandr assimp Xi dl)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)