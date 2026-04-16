#
# Try to find EGL library and include path.
# Once done this will define
#
# SFML_FOUND
# SFML_INCLUDE_PATH
# SFML_LIBRARY
#


#set(CMAKE_PREFIX_PATH $CMAKE_PREFIX_PATH "./../libs/SFML-3.0.2")
#set(SFML_DIR "./../libs/SFML-3.0.2/lib/cmake/SFML")
#set(SFML_INCLUDE_DIR "./../libs/SFML-3.0.2/include")
#set(SFML_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/../libs/SFML-3.0.2/lib")

set(SFML_STATIC_LIBRARIES ON)
find_package(SFML REQUIRED COMPONENTS System Window Graphics Network Audio)
message(" --\+++++++++ ${SFML_LIBRARIES} ")