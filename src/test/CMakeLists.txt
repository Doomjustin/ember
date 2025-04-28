function(add_ember_test name)
    add_executable(test.${name} test_${name}.cpp)
    target_link_libraries(test.${name}
        PRIVATE
            ember::ember
            ember::dependency
    )
    target_compile_features(test.${name} PRIVATE cxx_std_23)
endfunction()


add_ember_test(coding)
add_ember_test(write_batch)