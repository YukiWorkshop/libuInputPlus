cmake_minimum_required(VERSION 2.8)
project(libuInputPlus)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        uInput.cpp uInputSetup.cpp uInputResource.cpp
        uInput.hpp CommonIncludes.hpp uInputSetup.hpp)

add_library(uInputPlus SHARED ${SOURCE_FILES})
add_library(uInputPlus_Static STATIC ${SOURCE_FILES})
set_target_properties(uInputPlus_Static PROPERTIES OUTPUT_NAME uInputPlus)

add_executable(uInputPlus_test test.cpp)
target_link_libraries(uInputPlus_test uInputPlus)

install(TARGETS uInputPlus uInputPlus_Static DESTINATION lib)
install(FILES uInput.hpp uInputSetup.hpp CommonIncludes.hpp DESTINATION include/uInputPlus/)