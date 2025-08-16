cmake_minimum_required(VERSION 3.31)
project(nlohmann-example LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)
FetchContent_MakeAvailable(nlohmann_json)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE main.cpp)
target_compile_definitions(${PROJECT_NAME} PUBLIC JSON_DIAGNOSTICS=1)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
