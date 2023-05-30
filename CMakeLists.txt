cmake_minimum_required(VERSION 3.14)
project(GameFrame C CXX)

find_package(SFML COMPONENTS window REQUIRED)
find_package(glm REQUIRED)

file(GLOB_RECURSE GameFrame_SOURCES CONFIGURE_DEPENDS "GameFrame/**/*.hpp" "GameFrame/**/*.cpp")
add_library(GameFrame ${GameFrame_SOURCES})
target_link_libraries(GameFrame PRIVATE sfml-window glm)
target_compile_features(GameFrame PRIVATE cxx_std_17)
