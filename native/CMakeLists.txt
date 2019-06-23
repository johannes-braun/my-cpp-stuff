add_executable(native_executable)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
target_sources(native_executable PRIVATE ${SOURCES})

target_include_directories(native_executable PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)
target_link_libraries(native_executable PRIVATE processing)