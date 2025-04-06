add_subdirectory(acceptor)
add_subdirectory(event)

add_library(ember_tcp
    STATIC
        connection.cpp
        connection.h
        master.cpp
        master.h
        worker.cpp
        worker.h
)

add_library(ember::tcp ALIAS ember_tcp)

target_link_libraries(ember_tcp 
    PUBLIC 
        ember::base
        ember::tcp::event
        ember::tcp::acceptor
)