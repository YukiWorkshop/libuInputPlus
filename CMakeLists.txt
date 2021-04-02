cmake_minimum_required(VERSION 3.0)
project(libuInputPlus)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        uInput.cpp uInputSetup.cpp uInputResource.cpp
        uInput.hpp CommonIncludes.hpp uInputSetup.hpp)

include(GNUInstallDirs)

add_library(uInputPlus ${SOURCE_FILES})
target_include_directories(uInputPlus PUBLIC .)

add_executable(uInputPlus_test test.cpp)
target_link_libraries(uInputPlus_test uInputPlus)

configure_file(uInputPlus.pc.in uInputPlus.pc @ONLY)


install(TARGETS uInputPlus
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES uInput.hpp CommonIncludes.hpp uInputSetup.hpp
        DESTINATION include/)

