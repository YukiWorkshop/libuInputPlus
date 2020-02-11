cmake_minimum_required(VERSION 3.0)
project(libuInputPlus)

set(CMAKE_CXX_STANDARD 11)
set(PROJECT_VERSION "0.1.4")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "4")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "An easy-to-use C++ uinput library")
set(CPACK_PACKAGE_CONTACT "Yuki Workshop <https://github.com/YukiWorkshop>")
set(CMAKE_PROJECT_HOMEPAGE_URL "https://github.com/YukiWorkshop/libuInputPlus")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE ${CMAKE_PROJECT_HOMEPAGE_URL})
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.14), libstdc++6 (>= 5.2)")
set(CPACK_PACKAGE_FILE_NAME ${PROJECT_NAME}_${PROJECT_VERSION}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE})
set(CPACK_DEBIAN_DEBUGINFO_PACKAGE yes)
set(CPACK_DEBIAN_COMPRESSION_TYPE "xz")
set(CPACK_GENERATOR "DEB;RPM;TXZ")
set(CPACK_SOURCE_GENERATOR "DEB;RPM;TXZ")
include(CPack)

include(GNUInstallDirs)

set(SOURCE_FILES
        uInput.cpp uInputSetup.cpp uInputResource.cpp
        uInput.hpp CommonIncludes.hpp uInputSetup.hpp)

add_library(uInputPlus SHARED ${SOURCE_FILES})
add_library(uInputPlus_Static STATIC ${SOURCE_FILES})

set_target_properties(uInputPlus PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(uInputPlus PROPERTIES SOVERSION ${CPACK_PACKAGE_VERSION_MAJOR})
set_target_properties(uInputPlus_Static PROPERTIES OUTPUT_NAME uInputPlus)

add_executable(uInputPlus_test test.cpp)
target_link_libraries(uInputPlus_test uInputPlus)

configure_file(uInputPlus.pc.in uInputPlus.pc @ONLY)
install(TARGETS uInputPlus
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(TARGETS uInputPlus_Static
        DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES uInput.hpp uInputSetup.hpp CommonIncludes.hpp DESTINATION include/uInputPlus/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/uInputPlus.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig/)
