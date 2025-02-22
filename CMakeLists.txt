cmake_minimum_required(VERSION 3.5.0)
project(gravDash VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

SET(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin/)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

find_library(SFML_LIB sfml-graphics sfml-window sfml-system)
# Fetches the SFML library if it is not installed on the system
if (NOT SFML_LIB)
    include(FetchContent)
    FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
    FetchContent_MakeAvailable(SFML)
endif()

include_directories(include include/game include/menu include/utility)
file(GLOB SOURCES "src/*.cpp" "src/*/*.cpp")

add_executable(gravDash ${SOURCES})

target_link_libraries(gravDash PRIVATE sfml-graphics sfml-window sfml-system)
target_compile_features(gravDash PRIVATE cxx_std_20)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

if(WIN32)
    add_custom_command(
        TARGET gravDash
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:gravDash>
        VERBATIM)
endif()