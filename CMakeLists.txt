cmake_minimum_required(VERSION 2.8)

project(testing_lab)

add_subdirectory(gtest)

add_executable(tuple_testing tests.cpp tuple2.h)
target_link_libraries(tuple_testing gtest)
set_property(TARGET tuple_testing PROPERTY CXX_STANDARD 14)
if (WIN32)
	set_target_properties(tuple_testing PROPERTIES COMPILE_PDB_NAME "compiler1" COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}../tuple_testing.dir")
endif (WIN32)