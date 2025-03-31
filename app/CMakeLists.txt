add_executable(app main.cpp)

target_link_libraries(app PRIVATE base)

target_set_warnings(app ENABLE ALL AS_ERROR ALL DISABLE Annoying)
target_enable_lto(app optimized)
copy_dlls_for_debug(app "" "")


add_executable(asio_demo asio_demo.cpp)
target_link_libraries(asio_demo
    PRIVATE
        base
        Boost::headers
        Boost::asio
        Boost::system
        Boost::coroutine
        Boost::regex
)