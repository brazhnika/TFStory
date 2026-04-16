cmake_minimum_required(VERSION 4.2.3)

project(TFStoryTests)

add_executable(TFStoryTests)
target_compile_options(TFStoryTests PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
    -O2 
)

include(../FindSFML.cmake)

set(CMAKE_BUILD_TYPE Debug)

find_package(spdlog REQUIRED)

include_directories(
        ${PROJECT_SOURCE_DIR}/cfg
        ${PROJECT_SOURCE_DIR}/src
)

file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/../src/SortedSpritesList.cpp"
        "${PROJECT_SOURCE_DIR}/../src/SortedSpritesList.h"
        "${PROJECT_SOURCE_DIR}/tests/*.cpp"
        "${PROJECT_SOURCE_DIR}/tests/*.h"
        "${PROJECT_SOURCE_DIR}/tests/*.h"
        "${PROJECT_SOURCE_DIR}/../Utils/*.h"
        )

target_sources(TFStoryTests
   PRIVATE
     ${all_SRCS}
     SortedSpritesListTests.cpp
     SortTests.cpp
     TestData.h
)

enable_testing()


find_package(GTest CONFIG REQUIRED)

SET(CMAKE_C_FLAGS "-m32 -ggdb -D_DEBUG -DINICPP_DEBUG -Wextra -Wall -Wunreachable-code -fsanitize=address -O0 -D_LARGEFILE64_SOURCE")

if(SFML_FOUND)
    target_link_libraries(TFStoryTests PRIVATE -DINICPP_DEBUG -Wl,-Bstatic 
      SFML::Graphics SFML::Window SFML::System SFML::Network
      SFML::Audio GTest::gtest GTest::gmock gtest_main -Wl,-Bdynamic spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)
endif()

include(GoogleTest)
gtest_discover_tests(TFStoryTests)
