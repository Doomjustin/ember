function(add_ember_leveldb_test name)
    add_executable(leveldb.test.${name} test_${name}.cpp)
    target_link_libraries(leveldb.test.${name}
        PRIVATE
            ember::leveldb
            ember::dependency
    )
    target_compile_features(leveldb.test.${name} PRIVATE cxx_std_23)
endfunction()


add_ember_leveldb_test(coding)
add_ember_leveldb_test(write_batch)
add_ember_leveldb_test(log)
add_ember_leveldb_test(export)