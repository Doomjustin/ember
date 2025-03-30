add_library(ember_base INTERFACE)

add_library(ember::base ALIAS ember_base)

target_link_libraries(ember_base INTERFACE base)

target_include_directories(ember_base INTERFACE "${PROJECT_SOURCE_DIR}/include/")

target_compile_features(ember_base INTERFACE cxx_std_23)

target_compile_definitions(ember_base 
    INTERFACE
        $<$<CONFIG:Debug>:EMBER_DEBUG>
        $<$<CONFIG:Release>:DOCTEST_CONFIG_DISABLE>
)

configure_file(config.h.in ${PROJECT_SOURCE_DIR}/include/ember/config.h @ONLY)

add_subdirectory(net)

add_library(ember)

target_link_libraries(ember
    PUBLIC
        ember::net
    PRIVATE
        ember_base
)

add_library(ember::ember ALIAS ember)