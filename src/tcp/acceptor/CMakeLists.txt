add_library(ember_tcp_acceptor
    STATIC
        acceptor.cpp
        acceptor.h
        ipv4_acceptor.cpp
        ipv4_acceptor.h
        ipv6_acceptor.cpp
        ipv6_acceptor.h
)

add_library(ember::tcp::acceptor ALIAS ember_tcp_acceptor)

target_link_libraries(ember_tcp_acceptor PUBLIC ember::base)