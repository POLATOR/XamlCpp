project(XamlMarkup CXX)

file(GLOB XAML_MARKUP_HEADERS "include/xaml/markup/*.h")
file(GLOB XAML_MARKUP_SOURCES "src/*.cpp")

add_library(xaml_markup ${XAML_MARKUP_SOURCES})
target_link_libraries(xaml_markup PUBLIC xaml_meta)

target_include_directories(xaml_markup
	PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(${BUILD_SHARED_LIBS})
    target_compile_definitions(xaml_markup PRIVATE "XAML_MARKUP_API=__XAML_EXPORT")
endif()

if(${XAML_INSTALL})
    install(TARGETS xaml_markup)
    install(FILES $<TARGET_PDB_FILE:xaml_markup> DESTINATION bin)
    install(FILES ${XAML_MARKUP_HEADERS} DESTINATION include/xaml/markup)
endif()
