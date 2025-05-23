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

add_library(doctest INTERFACE)
target_compile_features(doctest INTERFACE cxx_std_23)
target_include_directories(doctest INTERFACE doctest/doctest)
add_library(doctest::doctest ALIAS doctest)


set(BUILD_TESTING ${BUILD_TESTING_BCKP} CACHE BOOL "Build tests (default variable for CTest)" FORCE) #Set it back to its past value

add_library(dependency INTERFACE)
add_library(ember::dependency ALIAS dependency)
target_compile_features(dependency INTERFACE cxx_std_23)
target_link_libraries(dependency
    INTERFACE
        gsl::gsl
        fmt::fmt
        spdlog::spdlog
        doctest::doctest
)