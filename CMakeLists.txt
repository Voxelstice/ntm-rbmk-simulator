cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)

project ("ntm-rbmk-simulator")

file(COPY ${CMAKE_SOURCE_DIR}/assets/ DESTINATION ${CMAKE_BINARY_DIR}/assets/)

# Set source files
set(MAIN_SOURCE
    src/main.cpp
    src/logger.cpp

    # libs
        src/libs/backward.cpp

    # classes
        src/classes/controlPanel.cpp
        src/classes/rbmk.cpp

        # columns
            src/classes/columns/column.cpp
)
set(MAIN_HEADERS
    src/main.h
    src/logger.h

    # libs
        src/libs/json.hpp
        src/libs/backward.hpp

    # classes
        src/classes/controlPanel.h
        src/classes/rbmk.h

        # columns
            src/classes/columns/column.h
)

add_executable(${PROJECT_NAME} ${MAIN_HEADERS} ${MAIN_SOURCE})

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Install raylib
set(RAYLIB_VERSION 5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

target_link_libraries(${PROJECT_NAME} raylib)