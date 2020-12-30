
cmake_minimum_required (VERSION 3.6)
project(jcord)

add_executable(jcord token.cpp fetcher.cpp cursor.cpp ui.cpp main.cpp)

add_subdirectory(libs/sleepy-discord)
target_link_libraries(jcord sleepy-discord)
target_link_libraries(jcord ncurses)
set(CMAKE_BUILD_TYPE Debug)
