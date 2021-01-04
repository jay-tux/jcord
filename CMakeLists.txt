
cmake_minimum_required (VERSION 3.6)
project(jcord)

add_executable(jcord src/token.cpp src/fetcher.cpp src/dmcache.cpp src/popup.cpp src/cursor.cpp src/ui.cpp src/main.cpp)

add_subdirectory(libs/sleepy-discord)
add_subdirectory(libs/ClipboardXX)
target_link_libraries(jcord sleepy-discord)
target_link_libraries(jcord ClipboardXX)
target_link_libraries(jcord ncurses)
set(CMAKE_BUILD_TYPE Debug)
