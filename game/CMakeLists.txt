add_executable(storm.game.demo demo.cpp)
target_link_libraries(storm.game.demo
    PRIVATE
        storm::base
        easyX::easyX
)

target_set_warnings(storm.game.demo ENABLE ALL AS_ERROR ALL DISABLE Annoying)
target_enable_lto(storm.game.demo optimized)
copy_dlls_for_debug(storm.game.demo "" "")