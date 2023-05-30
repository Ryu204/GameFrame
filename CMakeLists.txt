cmake_minimum_required(VERSION 3.21)
project(GameFrame C CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)


find_package(glm REQUIRED)

file(GLOB_RECURSE
    GameFrame_SOURCES
    CONFIGURE_DEPENDS
    "GameFrame/**/*.hpp"
    "GameFrame/**/*.cpp"
    "GameFrame/**/*.h"
    "GameFrame/**/*.c"
)

add_executable(GameFrame ${GameFrame_SOURCES})
target_link_libraries(GameFrame PRIVATE sfml-window glm::glm)
target_compile_features(GameFrame PRIVATE cxx_std_17)

if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET GameFrame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:GameFrame> $<TARGET_FILE_DIR:GameFrame> COMMAND_EXPAND_LISTS)
endif()