configure_file(config.cppm.in ${CMAKE_CURRENT_LIST_DIR}/config.cppm @ONLY)

add_library(ember_core)

add_library(ember::core ALIAS ember_core)

target_sources(ember_core
    PUBLIC FILE_SET CXX_MODULES
    FILES
        cache.cppm
        concurrency.queue.cppm
        config.cppm
        export.cppm
        generator.cppm
        noncopyable.cppm
        thread_pool.cppm
        utility.cppm
)

target_compile_features(ember_core PUBLIC cxx_std_23)

target_link_libraries(ember_core PRIVATE ember::dependency)

target_compile_definitions(ember_core
    INTERFACE
        $<$<CONFIG:Debug>:EMBER_DEBUG>
        $<$<CONFIG:Release>:DOCTEST_CONFIG_DISABLE>
)

if(${PROJECT_NAME}_BUILD_TESTS)
    add_subdirectory(test)
endif ()