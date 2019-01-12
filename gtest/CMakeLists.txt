add_library(gtest STATIC
  gtest/gtest.h
  gtest-all.cc
)

target_compile_definitions(gtest PUBLIC -Dgtest_disable_pthreads=ON)
# target_link_libraries(gtest pthread)
target_include_directories(gtest PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
