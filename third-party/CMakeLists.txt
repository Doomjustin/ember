set(BUILD_TESTING_BCKP ${BUILD_TESTING})
set(BUILD_TESTING OFF CACHE BOOL "Force disable of tests for external dependencies" FORCE)


add_subdirectory(GSL EXCLUDE_FROM_ALL)
target_set_warnings(GSL DISABLE ALL)
target_compile_features(GSL INTERFACE cxx_std_23)
add_library(gsl::gsl ALIAS GSL)

add_subdirectory(fmt EXCLUDE_FROM_ALL)
target_set_warnings(fmt DISABLE ALL)
target_compile_features(fmt INTERFACE cxx_std_23)

add_subdirectory(spdlog EXCLUDE_FROM_ALL)
target_compile_definitions(spdlog INTERFACE SPDLOG_FMT_EXTERNAL)
target_set_warnings(spdlog DISABLE ALL)
target_compile_features(spdlog INTERFACE cxx_std_23)

add_subdirectory(Catch2 EXCLUDE_FROM_ALL)


set(BUILD_TESTING ${BUILD_TESTING_BCKP} CACHE BOOL "Build tests (default variable for CTest)" FORCE) #Set it back to its past value

add_library(base INTERFACE)
target_compile_features(base INTERFACE cxx_std_23)
target_link_libraries(base
    INTERFACE
        gsl::gsl
        fmt::fmt
        spdlog::spdlog
)
