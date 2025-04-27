configure_file(config.h.in config.h @ONLY)


add_library(${PROJECT_NAME}_base INTERFACE)

add_library(${PROJECT_NAME}::base ALIAS ${PROJECT_NAME}_base)

target_link_libraries(${PROJECT_NAME}_base INTERFACE base)

target_include_directories(${PROJECT_NAME}_base INTERFACE ${PROJECT_SOURCE_DIR}/include/)

target_compile_features(${PROJECT_NAME}_base INTERFACE cxx_std_23)

target_compile_definitions(${PROJECT_NAME}_base 
    INTERFACE
        $<$<CONFIG:Debug>:EMBER_DEBUG>
        $<$<CONFIG:Release>:DOCTEST_CONFIG_DISABLE>
)


