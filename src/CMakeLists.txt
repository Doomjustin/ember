configure_file(config.cppm.in ${CMAKE_CURRENT_LIST_DIR}/ember.config.cppm @ONLY)

add_library(ember)

add_library(ember::ember ALIAS ember)

target_sources(ember
    PUBLIC FILE_SET CXX_MODULES
    FILES
        ember.config.cppm
        ember.noncopyable.cppm
        ember.leveldb.cppm
        ember.leveldb.options.cppm
        ember.leveldb.coding.cppm
        ember.leveldb.database.cppm
        ember.leveldb.write_batch.cppm
        ember.leveldb.leveldb.cppm
)

target_compile_features(ember PUBLIC cxx_std_23)

target_link_libraries(ember PRIVATE ember::dependency)

target_compile_definitions(ember
    INTERFACE
        $<$<CONFIG:Debug>:EMBER_DEBUG>
        $<$<CONFIG:Release>:DOCTEST_CONFIG_DISABLE>
)

if(${PROJECT_NAME}_BUILD_TESTS)
    add_subdirectory(test)
endif ()
