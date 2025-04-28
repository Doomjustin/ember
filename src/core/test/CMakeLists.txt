function(add_ember_core_test name)
    add_executable(core.test.${name} test_${name}.cpp)
    target_link_libraries(core.test.${name}
        PRIVATE
            ember::core
            ember::dependency
    )
    target_compile_features(core.test.${name} PRIVATE cxx_std_23)
endfunction()


add_ember_core_test(thread_pool)
add_ember_core_test(lru_cache)
add_ember_core_test(utility)
add_ember_core_test(generator)
