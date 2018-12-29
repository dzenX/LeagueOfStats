cmake_minimum_required( VERSION 3.1 )

project( LeagueOfStats )
find_package( CURL )

set ( CMAKE_CXX_STANDART 14 )
set ( CMAKE_CXX_STANDART_REQUIRED ON )

include ( ExternalProject )

ExternalProject_Add ( cpr 
                    GIT_REPOSITORY https://github.com/whoshuu/cpr.git
                    GIT_TAG master
                    GIT_SUBMODULES cpr
                    INSTALL_COMMAND ""
                    CMAKE_ARGS -DUSE_SYSTEM_CURL=ON -DBUILD_CPR_TESTS=OFF )

ExternalProject_Get_property( cpr SOURCE_DIR BINARY_DIR )

include_directories ( ${SOURCE_DIR}/include )

file ( GLOB CPP_FILES main.cpp Server.cpp PLayer.cpp Game.cpp )

add_executable ( ${CMAKE_PROJECT_NAME} ${CPP_FILES} )

add_dependencies ( ${CMAKE_PROJECT_NAME} cpr )

target_link_libraries ( ${CMAKE_PROJECT_NAME} ${BINARY_DIR}/lib/libcpr.a curl )