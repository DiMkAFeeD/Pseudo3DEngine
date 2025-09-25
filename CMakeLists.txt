cmake_minimum_required(VERSION 3.10)

project(Pseudo3DEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "C:/path/SFML/lib/cmake/SFML")

include_directories("C:/path/include")

find_package(SFML 3.0 REQUIRED COMPONENTS Graphics Window System Network)

set(src
    main.cpp
 "World.h" "World.cpp")

add_executable(${PROJECT_NAME} ${src})

target_link_libraries(${PROJECT_NAME} PRIVATE 
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Network
)

# --- Копирование папки assets ---
set(ASSETS_SRC "${CMAKE_SOURCE_DIR}/assets")
set(ASSETS_DST "${CMAKE_BINARY_DIR}/assets")

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory "${ASSETS_DST}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${ASSETS_SRC}" "${ASSETS_DST}"
    COMMENT "Копирование assets всегда"
)
add_dependencies(${PROJECT_NAME} copy_assets)
