cmake_minimum_required(VERSION 3.10)

project(Flappy_bird_AI)

file(GLOB PROJ_SRC
    "sources/*.cpp"
    "sources/materials/*.cpp"
    "includes/*.h"
    "includes/materials/*.h"
)
if ((NOT DEFINED VCPKG_TARGET_TRIPLET) AND ("${CMAKE_CXX_COMPILER}" MATCHES  g\\+\\+\\.exe) AND (${CMAKE_SIZEOF_VOID_P} EQUAL 4))
    message("Configuration pour MinGW 32")
    set(SFML_DIR "${CMAKE_SOURCE_DIR}/dependencies/SFML/lib/cmake/SFML" CACHE FILEPATH "The directory containing a CMake configuration file for SFML.")
endif()

add_executable(Flappy_bird_AI ${PROJ_SRC})
target_include_directories(Flappy_bird_AI PRIVATE ${CMAKE_SOURCE_DIR}/includes)

find_package(SFML COMPONENTS system window graphics REQUIRED)
target_link_libraries(Flappy_bird_AI PRIVATE
        sfml-system
        sfml-main
        sfml-window
        sfml-graphics
    )
target_include_directories(Flappy_bird_AI PRIVATE ${SFML_INCLUDE_DIR})
