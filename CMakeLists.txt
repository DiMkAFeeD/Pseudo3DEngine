cmake_minimum_required(VERSION 3.10)

project(Pseudo3DEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ѕуть до SFML (можно убрать, если SFML установлен глобально)
set(SFML_DIR "C:/path/SFML/lib/cmake/SFML")

find_package(SFML 3.0 REQUIRED COMPONENTS Graphics Window System Network)

set(src
    main.cpp
)

add_executable(${PROJECT_NAME} ${src})

target_link_libraries(${PROJECT_NAME} PRIVATE 
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Network
)
