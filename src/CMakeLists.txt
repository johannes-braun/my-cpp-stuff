add_library(processing SHARED)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
target_sources(processing PRIVATE ${SOURCES})
target_include_directories(processing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)
target_compile_features(processing PUBLIC cxx_std_17)
target_link_libraries(processing PUBLIC libraries_interface)