cmake_minimum_required(VERSION 3.28.3)
project(FOrMit LANGUAGES C CXX)

include(cmake/CMakeConfiguration.cmake)
include(cmake/CMakeDependencies.cmake)
include(cmake/CMakeFindFiles.cmake)
add_executable(FOrMit ${C_FILES} ${CXX_FILES} ${H_FILES})
include(cmake/CMakeLinks.cmake)