cmake_minimum_required(VERSION 3.5.0)
project(gravDash.app VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

SET(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin/)
include_directories(include)
file(GLOB SOURCES "src/*.cpp")

#add_library(GravDashObject.o OBJECT ${SOURCES})

add_executable(gravDash.app ${SOURCES})

target_link_libraries(gravDash.app PRIVATE sfml-graphics sfml-window sfml-system)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)