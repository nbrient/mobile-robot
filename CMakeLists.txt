cmake_minimum_required(VERSION 3.16)
project(robot_target LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2 REQUIRED COMPONENTS graphics window system)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(robot_target
    src/main.cpp
    src/app/application.cpp
    src/app/config.cpp
    src/map/map.cpp
    src/map/mapGenerator.cpp
    src/entity/target.cpp
    src/graphics/renderer.cpp
    src/graphics/hud.cpp
    src/entity/robot.cpp
    src/nav/pathPlanner.cpp
    src/nav/bfsPathPlanner.cpp
)

target_include_directories(robot_target
    PRIVATE
    inc
)

target_link_libraries(robot_target
    PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    nlohmann_json::nlohmann_json
)
