project(XamlMetaTest CXX)

file(GLOB TEST_SOURCE "src/*.cpp")
add_executable(meta_test ${TEST_SOURCE})
find_package(sf REQUIRED)
find_package(nowide REQUIRED)
target_link_libraries(meta_test xaml_meta stream_format nowide::nowide)
target_include_directories(meta_test PUBLIC include)
