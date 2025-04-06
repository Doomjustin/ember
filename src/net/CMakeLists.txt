add_library(ember_net 
    STATIC
        acceptor/acceptor.cpp
        acceptor/acceptor.h
        acceptor/ipv4_acceptor.cpp
        acceptor/ipv4_acceptor.h
        acceptor/ipv6_acceptor.cpp
        acceptor/ipv6_acceptor.h
        event/scheduler/poller.cpp
        event/scheduler/poller.h
        event/scheduler/selecter.cpp
        event/scheduler/selecter.h
        event/count_event.cpp
        event/count_event.h
        event/event_builder.cpp
        event/event_builder.h
        event/event_scheduler.cpp
        event/event_scheduler.h
        event/event.cpp
        event/event.h
        event/forever_event.h
        event/once_event.h
        event/scheduler_noticifier.cpp
        event/scheduler_noticifier.h
        http/http_request.cpp
        http/http_request.h
        http/http_response.cpp
        http/http_response.h
        http/http_server.cpp
        http/http_server.h
        connection.cpp
        connection.h
        defination.h
        endpoint.h
        configuration.cpp
        configuration.h
        master.cpp
        master.h
        posix.cpp
        posix.h
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
