project(XamlUIAppMain CXX)

file(GLOB APPMAIN_SOURCE "src/*.cpp")

add_library(xaml_ui_appmain OBJECT ${APPMAIN_SOURCE})

find_package(sf REQUIRED)
find_package(nowide REQUIRED)
target_link_libraries(xaml_ui_appmain PUBLIC xaml_ui PRIVATE stream_format nowide::nowide)

if(WIN32 AND MSVC)
	target_link_options(xaml_ui_appmain PUBLIC "/entry:mainCRTStartup")
endif()
