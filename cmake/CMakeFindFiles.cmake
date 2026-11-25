include_directories(${PROJECT_SOURCE_DIR}/includes)

file(GLOB_RECURSE C_FILES   ${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE CXX_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE H_FILES   ${PROJECT_SOURCE_DIR}/includes/*.h)