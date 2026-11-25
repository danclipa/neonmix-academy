cmake_minimum_required(VERSION 3.20)

project(NeonMixAcademy LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(httplib CONFIG REQUIRED)

add_executable(neonmix src/main.cpp)
target_link_libraries(neonmix PRIVATE httplib::httplib)

if(MSVC)
    target_compile_options(neonmix PRIVATE /W4 /permissive-)
else()
    target_compile_options(neonmix PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_custom_target(academy_templates ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/templates"
        "${CMAKE_BINARY_DIR}/templates"
    COMMENT "Copying Academy templates"
)
add_dependencies(neonmix academy_templates)
