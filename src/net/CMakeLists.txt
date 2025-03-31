add_library(ember_net 
    STATIC
        endpoint.h
        posix.cpp
        posix.h
        socket.cpp
        socket.h
)

target_include_directories(ember_net INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(ember_net
    PUBLIC
        ember_base
)

add_library(ember::net ALIAS ember_net)
