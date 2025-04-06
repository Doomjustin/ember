add_library(ember_base
    STATIC
        config.h
        defination.h
        endpoint.h
        noncopyable.h
        posix.cpp
        posix.h
        socket.cpp
        socket.h
)

add_library(ember::base ALIAS ember_base)

target_link_libraries(ember_base PUBLIC base)

target_include_directories(ember_base PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_compile_features(ember_base PUBLIC cxx_std_23)

target_compile_definitions(ember_base 
    PUBLIC
        $<$<CONFIG:Debug>:EMBER_DEBUG>
        $<$<CONFIG:Release>:DOCTEST_CONFIG_DISABLE>
)

configure_file(config.h.in ${CMAKE_CURRENT_LIST_DIR}/config.h @ONLY)

add_subdirectory(tcp)
add_subdirectory(http)

add_library(ember)

target_link_libraries(ember
    PUBLIC
        ember::tcp
        ember::http
        ember::base
)

add_library(ember::ember ALIAS ember)