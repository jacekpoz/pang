cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME pang)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

project(${PROJECT_NAME} VERSION 0.0.1
			 DESCRIPTION "recreation of the arcade pang game for the motorola science cup 2021"
			 LANGUAGES CXX)

set(SFML_DIR "/usr/include/SFML/")
find_package(SFML 2 REQUIRED COMPONENTS audio graphics window system)

add_executable(${PROJECT_NAME} 
	main.cpp
	Game.cpp
	FPS.cpp
	systems/CollisionSystem.cpp
	systems/GravitySystem.cpp
	systems/PlayerMovementSystem.cpp
	systems/RenderingSystem.cpp
	systems/System.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC "../lib/entt/src")
target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
