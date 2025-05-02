add_library(ember_leveldb)

add_library(ember::leveldb ALIAS ember_leveldb)

target_sources(ember_leveldb
    PUBLIC FILE_SET CXX_MODULES
    FILES
        coding.cppm
        database.cppm
        exceptions.cppm
        export.cppm
        file_mutex.cppm
        filename.cppm
        leveldb.cppm
        log.cppm
        options.cppm
        version.cppm
        write_batch.cppm
)

target_compile_features(ember_leveldb PUBLIC cxx_std_23)

target_link_libraries(ember_leveldb 
    PRIVATE 
        ember::core
        ember::dependency
)

target_compile_definitions(ember_leveldb
    INTERFACE
        $<$<CONFIG:Debug>:EMBER_DEBUG>
        $<$<CONFIG:Release>:DOCTEST_CONFIG_DISABLE>
)

if(${PROJECT_NAME}_BUILD_TESTS)
    add_subdirectory(test)
endif ()