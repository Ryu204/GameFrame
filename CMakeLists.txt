cmake_minimum_required(VERSION 3.21)
project(GameFrame C CXX)

find_package(SFML COMPONENTS window REQUIRED)
find_package(glm REQUIRED)
find_package(efsw REQUIRED)

file(GLOB_RECURSE
    GameFrame_SOURCES
    CONFIGURE_DEPENDS
    "GameFrame/**/*.hpp"
    "GameFrame/**/*.cpp"
    "GameFrame/**/*.h"
    "GameFrame/**/*.c"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(GameFrame ${GameFrame_SOURCES})

# add_library(GameFrame ${GameFrame_SOURCES})
target_link_libraries(GameFrame PRIVATE sfml-window glm::glm efsw::efsw)
