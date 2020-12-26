
cmake_minimum_required (VERSION 3.6)
project(jcord)

add_executable(jcord main.cpp)

add_subdirectory(libs/sleepy-discord)
target_link_libraries(jcord sleepy-discord)
