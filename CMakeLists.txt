cmake_minimum_required(VERSION 4.2.3)

project(TFStory)

add_executable(TFStory)

target_compile_options(TFStory PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)

include(FindSFML.cmake)

set(CMAKE_BUILD_TYPE Debug)

find_package(spdlog REQUIRED)

include_directories(
        ${PROJECT_SOURCE_DIR}/cfg
        ${PROJECT_SOURCE_DIR}/src
)

file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/cfg/*.h"
        "${PROJECT_SOURCE_DIR}/cfg/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/*.c"
        "${PROJECT_SOURCE_DIR}/src/*.h"
        "${PROJECT_SOURCE_DIR}/src/menu/*.h"
        "${PROJECT_SOURCE_DIR}/src/menu/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/controls/*.h"
        "${PROJECT_SOURCE_DIR}/src/controls/*.cpp"        
        "${PROJECT_SOURCE_DIR}/src/dlg/*.h"
        "${PROJECT_SOURCE_DIR}/src/dlg/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/ResourceManagers/*.h"
        "${PROJECT_SOURCE_DIR}/src/ResourceManagers/*.cpp"
        )

target_sources(TFStory
   PRIVATE
     ${all_SRCS}
     main.cpp
)

SET(CMAKE_C_FLAGS "-m32 -ggdb -D_DEBUG -DINICPP_DEBUG -Wextra -Wall -Wunreachable-code -fsanitize=address -O0 -D_LARGEFILE64_SOURCE")

if(SFML_FOUND)
    target_link_libraries(TFStory PRIVATE -DINICPP_DEBUG -Wl,-Bstatic 
      SFML::Graphics SFML::Window SFML::System SFML::Network
      SFML::Audio -Wl,-Bdynamic spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)
endif()
