cmake_minimum_required(VERSION 4.0.0)

project(EMBER
    VERSION 0.0.1
    LANGUAGES C CXX
)

include(CTest)
include(CMakeDependentOption)

cmake_dependent_option(${PROJECT_NAME}_BUILD_TESTS
    "Enable ${PROJECT_NAME} project tests targets" ON
    "BUILD_TESTING" OFF
)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(ConfigSafeGuards)
include(Colors)
include(Warnings)
include(LTO)
include(Coverage)
include(CopyDllsForDebug)

find_lto(CXX)

add_subdirectory(third-party)

add_subdirectory(app)
add_subdirectory(src)

add_subdirectory(exercise)

if(${PROJECT_NAME}_BUILD_TESTS)
    set(TEST_RUNNER_PARAMS "--force-colors=true" CACHE STRING "Options to add to our test runners commands")
    list(APPEND CUSTOM_COVERAGE_EXCLUDE "/test/")

    configure_file(
        ${CMAKE_CURRENT_LIST_DIR}/cmake/CTestCustom.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/CTestCustom.cmake
        @ONLY
    )
endif()