function(add_net_test target)
    add_executable(test.${target} ${target}.cpp)
    target_link_libraries(test.${target} 
        PRIVATE 
            ember::net
            doctest::doctest
    )

    add_test(
        NAME ember.test.${target}
        COMMAND test.${target} ${TEST_RUNNER_PARAMS}
    )
endfunction()


add_net_test(check_result)

add_executable(daytime_server daytime.cpp)
target_link_libraries(daytime_server
    PRIVATE 
        ember::net
)
