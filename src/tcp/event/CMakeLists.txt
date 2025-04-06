add_library(ember_tcp_event
    STATIC
        scheduler/poller.cpp
        scheduler/poller.h
        scheduler/selecter.cpp
        scheduler/selecter.h
        count_event.cpp
        count_event.h
        event_builder.cpp
        event_builder.h
        event_scheduler.cpp
        event_scheduler.h
        event.cpp
        event.h
        forever_event.h
        once_event.h
        scheduler_noticifier.cpp
        scheduler_noticifier.h
)

add_library(ember::tcp::event ALIAS ember_tcp_event)

target_link_libraries(ember_tcp_event PUBLIC ember::base)