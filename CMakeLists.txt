cmake_minimum_required(VERSION 3.13)
project(mpp
    VERSION 0.1
    LANGUAGES CXX C)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(mpp ${SOURCES})
target_include_directories(mpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)
target_compile_features(mpp PUBLIC cxx_std_17)

# Libraries
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)

target_link_libraries(mpp PUBLIC glm glfw)
target_compile_definitions(mpp PUBLIC GLM_FORCE_PURE)
target_compile_options(mpp PUBLIC $<$<CXX_COMPILER_ID:MSVC>:/permissive->)

target_link_libraries(mpp PRIVATE spdlog::spdlog)
target_link_libraries(mpp PRIVATE fmt::fmt fmt::fmt-header-only)