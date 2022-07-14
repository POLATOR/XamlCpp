project(GlobalTestCXX CXX)

file(GLOB TEST_SOURCE "src/*.cpp")

add_executable(global_test_cxx ${TEST_SOURCE})
find_package(sf REQUIRED)
find_package(nowide REQUIRED)
target_link_libraries(global_test_cxx xaml_global stream_format nowide::nowide)
