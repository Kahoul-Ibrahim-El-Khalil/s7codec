cmake_minimum_required(VERSION 3.10)
project(s7_s7_library VERSION 1.0.0 LANGUAGES CXX)

add_library(s7 INTERFACE)
add_library(s7codec::s7 ALIAS s7)

target_include_directories(s7 INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Optional: Install rules
install(TARGETS s7 EXPORT s7Targets)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include)

install(EXPORT s7Targets
        FILE s7Targets.cmake
        NAMESPACE s7codec::
        DESTINATION lib/cmake/s7)
