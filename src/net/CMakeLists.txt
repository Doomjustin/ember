add_library(ember_net 
    STATIC
        accepter.cpp
        accepter.h
        connection.cpp
        connection.h
        count_event.cpp
        count_event.h
        endpoint.h
        event_builder.cpp
        event_builder.h
        event_loop.h
        event_scheduler.cpp
        event_scheduler.h
        event.cpp
        event.h
        forever_event.h
        ipv4_accepter.cpp
        ipv4_accepter.h
        ipv6_accepter.cpp
        ipv6_accepter.h
        once_event.h
        posix.cpp
        posix.h
        protocol.h
        selecter.cpp
        selecter.h
        socket.cpp
        socket.h
)

target_include_directories(ember_net INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(ember_net
    PUBLIC
        ember_base
)

add_library(ember::net ALIAS ember_net)
