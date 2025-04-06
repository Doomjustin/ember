add_library(ember_net 
    STATIC
        acceptor.cpp
        acceptor.h
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
        ipv4_acceptor.cpp
        ipv4_acceptor.h
        ipv6_acceptor.cpp
        ipv6_acceptor.h
        master.cpp
        master.h
        once_event.h
        posix.cpp
        posix.h
        protocol.h
        scheduler_noticifier.cpp
        scheduler_noticifier.h
        selecter.cpp
        selecter.h
        socket.cpp
        socket.h
        worker.cpp
        worker.h
)

target_include_directories(ember_net INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(ember_net
    PUBLIC
        ember_base
)

add_library(ember::net ALIAS ember_net)
